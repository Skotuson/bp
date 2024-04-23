#include "Instruction.hpp"

#include "../interpreter/data_structures/word/ListWord.hpp"

#include <iostream>
#include <cassert>
#include <memory>

void Instruction::fail(WAMState &state)
{
    std::shared_ptr<FailInstruction> fail = std::make_shared<FailInstruction>();
    fail->execute(state);
}

void Instruction::clearPDL(WAMState &state, std::shared_ptr<Word> X, std::shared_ptr<Word> Y)
{
    // Make static for class
    std::vector<std::vector<size_t>> table = {
        {1, 1, 1, 1, 1},
        {2, 3, 5, 5, 5},
        {2, 4, 6, 0, 0},
        {2, 4, 0, 7, 0},
        {2, 4, 0, 0, 8}};

    while (42)
    {
        size_t branch = table[X->tag()][Y->tag()];
        // X is a ref, dereference:
        if (branch == 1)
        {
            X = X->dereference();
            continue;
        }

        // Y is a ref, dereference:
        else if (branch == 2)
        {
            Y = Y->dereference();
            continue;
        }

        // X and Y are both unbound variables
        else if (branch == 3)
        {
            std::shared_ptr<VariableWord> x = std::static_pointer_cast<VariableWord>(X);
            std::shared_ptr<VariableWord> y = std::static_pointer_cast<VariableWord>(Y);
            // Trail both X and Y
            state.trailPush(x);
            state.trailPush(y);
            // Bind them together
            // TODO: bind both
            // x->bind();
            y->bind(X);
        }

        // X is not a variable, Y is an unbound variable
        else if (branch == 4)
        {
            std::shared_ptr<VariableWord> vw = std::static_pointer_cast<VariableWord>(Y);
            // Trail
            state.trailPush(vw);
            //*vw->ref() = X->clone();
            vw->bind(X->clone());
        }

        // Y is a constant, X is an unbound variable
        else if (branch == 5)
        {
            std::shared_ptr<VariableWord> vw = std::static_pointer_cast<VariableWord>(X);
            // Trail
            state.trailPush(vw);
            //*vw->ref() = Y->clone();
            vw->bind(Y->clone());
        }

        // Both X and Y are constants
        else if (branch == 6)
        {
            std::shared_ptr<ConstantWord> xc = std::static_pointer_cast<ConstantWord>(X);
            if (!Y->compareToConst(xc))
            {
                fail(state);
            }
        }

        else if (branch == 7)
        {
            size_t XA = std::static_pointer_cast<ListWord>(X)->m_HeapAddress;
            size_t YA = std::static_pointer_cast<ListWord>(Y)->m_HeapAddress;
            state.pdlPush({XA, YA, 2});
        }

        else if (branch == 8)
        {
            std::shared_ptr<StructurePointerWord> x = std::static_pointer_cast<StructurePointerWord>(X);
            std::shared_ptr<StructurePointerWord> y = std::static_pointer_cast<StructurePointerWord>(Y);
            std::shared_ptr<StructureWord> xs = std::static_pointer_cast<StructureWord>(state.heapAt(x->m_HeapAddress));
            std::shared_ptr<StructureWord> ys = std::static_pointer_cast<StructureWord>(state.heapAt(y->m_HeapAddress));

            if (!xs->compareToStruct(ys))
            {
                fail(state);
                break;
            }

            state.pdlPush({x->m_HeapAddress + 1, y->m_HeapAddress + 1, xs->m_Arity});
        }

        // Fail branch
        else
        {
            fail(state);
            break;
        }

        // If PDL is empty, terminate the loop
        if (state.pdlEmpty())
        {
            break;
        }

        // Get information from the PDL and repeat the loop
        else
        {
            PDLTriple pdlTriple = state.pdlTop();
            size_t XA = std::get<0>(pdlTriple);
            size_t YA = std::get<1>(pdlTriple);
            X = state.heapAt(XA);
            Y = state.heapAt(YA);
            size_t N = std::get<2>(pdlTriple) - 1;
            state.pdlPop();

            if (N)
            {
                state.pdlPush({XA + 1, YA + 1, N});
            }
        }
    }
}

std::ostream &operator<<(std::ostream &os, const Instruction &instr)
{
    instr.print(os);
    return os;
}

// Indexing Instructions

std::shared_ptr<Instruction> MarkInstruction::clone(void)
{
    return std::make_shared<MarkInstruction>();
}

BranchInstruction::BranchInstruction(const std::string &label, size_t address)
    : m_Label(label),
      m_Address(address)
{
}

void BranchInstruction::setAddress(size_t address)
{
    m_Address = address;
}

void MarkInstruction::execute(WAMState &state)
{
    // Build a new choice point up to the enviornment
    auto ncp = std::make_shared<ChoicePoint>(state.m_ArgumentRegisters,
                                             state.EReg(),
                                             state.m_ContinuationPointer,
                                             state.BReg(),
                                             state.TRReg(),
                                             state.HReg(),
                                             state.m_ProgramCounter);
    state.stackPush(ncp);
    // Set E and B registers
    // Make it a current one
    state.m_BacktrackRegister = state.m_EnvironmentRegister = state.SReg() - 1;
}

void MarkInstruction::print(std::ostream &os) const
{
    os << "mark";
}

RetryMeElseInstruction::RetryMeElseInstruction(const std::string &label, size_t address)
    : BranchInstruction(label, address)
{
}

std::shared_ptr<Instruction> RetryMeElseInstruction::clone(void)
{
    return std::make_shared<RetryMeElseInstruction>(m_Label, m_Address);
}

void RetryMeElseInstruction::execute(WAMState &state)
{
    // Set next clause to the L (m_Address)
    std::shared_ptr<ChoicePoint> cp = state.stackAt(state.BReg());
    if (cp)
    {
        cp->m_FA = m_Address;
    }
}

void RetryMeElseInstruction::print(std::ostream &os) const
{
    os << "retry-me-else " << m_Label << "[" << m_Address << "]";
}

std::shared_ptr<Instruction> BacktrackInstruction::clone(void)
{
    return std::make_shared<BacktrackInstruction>();
}

void BacktrackInstruction::execute(WAMState &state)
{
    std::shared_ptr<ChoicePoint> cp = state.stackAt(state.m_BacktrackRegister);
    if (cp)
    {
        state.m_BacktrackRegister = cp->m_BB;
    }

    fail(state);

    // Discard the choice point (last clause in the chain failed)
    state.stackPop();
}

void BacktrackInstruction::print(std::ostream &os) const
{
    os << "backtrack";
}

std::shared_ptr<Instruction> FailInstruction::clone(void)
{
    return std::make_shared<FailInstruction>();
}

void FailInstruction::execute(WAMState &state)
{
    std::shared_ptr<ChoicePoint> cp = state.stackAt(state.m_BacktrackRegister);
    if (cp)
    {
        // Reload argument registers
        state.m_ArgumentRegisters = cp->m_ArgumentRegisters;

        // Reset heap
        while (state.HReg() != cp->m_BH)
        {
            state.heapPop();
        }

        // Reset all variables instantiated since choice point was built
        while (state.TRReg() != cp->m_BTR)
        {
            std::shared_ptr<VariableWord> popped = state.trailTop();
            popped->unbind();
            *popped->ref() = popped;
            state.trailPop();
        }
        // Branch to next rule
        state.m_ProgramCounter = cp->m_FA;
    }
    // TODO: Experimental (check when choice point stack is empty)
    else if (state.m_BacktrackRegister == UNSET_REG)
        state.m_FailFlag = true;
}

void FailInstruction::print(std::ostream &os) const
{
    os << "__fail__";
}

// Procedural Instructions
AllocateInstruction::AllocateInstruction(size_t n)
    : m_N(n)
{
}

std::shared_ptr<Instruction> AllocateInstruction::clone(void)
{
    return std::make_shared<AllocateInstruction>(m_N);
}

void AllocateInstruction::execute(WAMState &state)
{
    std::shared_ptr<ChoicePoint> cp = state.stackTop();
    // Allocate new environment to current choice point (Initialize all variables).
    cp->m_Variables.resize(m_N, nullptr);
    for (size_t i = 0; i < m_N; i++)
    {
        if (state.m_QueryVariables.count(i))
        {
            cp->m_Variables[i] = std::make_shared<VariableWord>(&cp->m_Variables[i], state.m_QueryVariables[i]);
        }
        else
        {
            cp->m_Variables[i] = std::make_shared<VariableWord>(&cp->m_Variables[i]);
        }
    }
    // Set E to this choice point
    state.m_EnvironmentRegister = state.BReg();
}

void AllocateInstruction::print(std::ostream &os) const
{
    os << "allocate " << m_N;
}

CallInstruction::CallInstruction(const std::string &label, size_t address)
    : BranchInstruction(label, address)
{
}

std::shared_ptr<Instruction> CallInstruction::clone(void)
{
    return std::make_shared<CallInstruction>(m_Label, m_Address);
}

void CallInstruction::execute(WAMState &state)
{
    // Program counter already points to another instruction
    state.m_ContinuationPointer = state.m_ProgramCounter;
    // Branch to L (m_Address), with return address in CP.
    state.m_ProgramCounter = m_Address;
}

void CallInstruction::print(std::ostream &os) const
{
    os << "call " + m_Label << "[" << m_Address << "]";
}

std::shared_ptr<Instruction> ReturnInstruction::clone(void)
{
    return std::make_shared<ReturnInstruction>();
}

void ReturnInstruction::execute(WAMState &state)
{
    std::shared_ptr<ChoicePoint> cp = state.stackAt(state.EReg());
    if (cp)
    {
        // Caller's return address
        state.m_ProgramCounter = cp->m_BCP;
        // Caller's environment
        state.m_EnvironmentRegister = cp->m_BCE;
    }
    else
    {
        // TODO: mainly debug
        throw std::runtime_error("Invalid stack access");
    }
    // std::cout << state << std::endl;
}

void ReturnInstruction::print(std::ostream &os) const
{
    os << "return";
}

// Get Instructions

GetInstruction::GetInstruction(const std::string &name, size_t argumentRegister)
    : m_Name(name),
      m_ArgumentRegister(argumentRegister)
{
}

GetConstantInstruction::GetConstantInstruction(const std::string &name, size_t argumentRegister)
    : GetInstruction(name, argumentRegister)
{
}

std::shared_ptr<Instruction> GetConstantInstruction::clone(void)
{
    return std::make_shared<GetConstantInstruction>(m_Name, m_ArgumentRegister);
}

void GetConstantInstruction::execute(WAMState &state)
{
    std::shared_ptr<Word> reg = state.m_ArgumentRegisters.dereferenceRegister(m_ArgumentRegister);
    std::shared_ptr<ConstantWord> cword = std::make_shared<ConstantWord>(m_Name);
    if (reg && reg->tag() == TAG::VARIABLE)
    {
        std::shared_ptr<VariableWord> vw = std::static_pointer_cast<VariableWord>(reg);
        state.trailPush(vw); // Trail
        //*vw->ref() = cword;
        vw->bind(cword);
    }
    else if (!reg || !reg->compareToConst(cword))
    {
        fail(state);
    }
}

void GetConstantInstruction::print(std::ostream &os) const
{
    os << "get-constant " << m_Name << " A" << m_ArgumentRegister;
}

GetListInstruction::GetListInstruction(const std::string &name, size_t argumentRegister)
    : GetInstruction(name, argumentRegister)
{
}

std::shared_ptr<Instruction> GetListInstruction::clone(void)
{
    return std::make_shared<GetListInstruction>(m_Name, m_ArgumentRegister);
}

void GetListInstruction::execute(WAMState &state)
{
    std::shared_ptr<Word> w = state.m_ArgumentRegisters.dereferenceRegister(m_ArgumentRegister);
    if (w->tag() == VARIABLE)
    {
        std::shared_ptr<VariableWord> vw = std::static_pointer_cast<VariableWord>(w);
        state.trailPush(vw);
        vw->bind(std::make_shared<ListWord>(state.HReg(), state.m_Heap));
        state.setWriteMode();
    }
    else if (w->tag() == LIST)
    {
        std::shared_ptr<ListWord> lw = std::static_pointer_cast<ListWord>(w);
        state.m_StructurePointer = lw->m_HeapAddress;
        state.setReadMode();
    }
    else
    {
        fail(state);
    }
}

void GetListInstruction::print(std::ostream &os) const
{
    os << "get-list A" << m_ArgumentRegister;
}

GetStructureInstruction::GetStructureInstruction(const std::string &name, size_t argumentRegister, size_t arity)
    : GetInstruction(name, argumentRegister),
      m_Arity(arity)
{
}

std::shared_ptr<Instruction> GetStructureInstruction::clone(void)
{
    return std::make_shared<GetStructureInstruction>(m_Name, m_ArgumentRegister, m_Arity);
}

void GetStructureInstruction::execute(WAMState &state)
{
    std::shared_ptr<Word> w = state.m_ArgumentRegisters.dereferenceRegister(m_ArgumentRegister);
    if (w->tag() == VARIABLE)
    {
        std::shared_ptr<VariableWord> vw = std::static_pointer_cast<VariableWord>(w);
        state.trailPush(vw);
        //*vw->ref() = new StructurePointerWord(state.HReg(), state.m_Heap);
        vw->bind(std::make_shared<StructurePointerWord>(state.HReg(), state.m_Heap));
        state.heapPush(std::make_shared<StructureWord>(m_Name, m_Arity, state.m_Heap, state.HReg()));
        state.setWriteMode();
    }

    else if (w->tag() == S_POINTER)
    {
        std::shared_ptr<StructurePointerWord> spw = std::static_pointer_cast<StructurePointerWord>(w);
        std::shared_ptr<StructureWord> sw = std::static_pointer_cast<StructureWord>(state.heapAt(spw->m_HeapAddress));
        if (sw->m_Functor == m_Name && sw->m_Arity == m_Arity)
        {
            state.setReadMode();
            state.m_StructurePointer = spw->m_HeapAddress + 1;
            return;
        }
        fail(state);
    }

    else
    {
        fail(state);
    }
}

void GetStructureInstruction::print(std::ostream &os) const
{
    os << "get-structure " << m_Name << "/" << m_Arity << " A" << m_ArgumentRegister;
}

GetVariableInstruction::GetVariableInstruction(const std::string &name,
                                               size_t argumentRegister, size_t offset)
    : GetInstruction(name, argumentRegister),
      m_Offset(offset)
{
}

std::shared_ptr<Instruction> GetVariableInstruction::clone(void)
{
    return std::make_shared<GetVariableInstruction>(m_Name, m_ArgumentRegister, m_Offset);
}

void GetVariableInstruction::execute(WAMState &state)
{
    std::shared_ptr<Word> X = state.m_ArgumentRegisters.dereferenceRegister(m_ArgumentRegister),
                          Y = state.stackAt(state.EReg())->m_Variables[m_Offset];
    clearPDL(state, X, Y);
}

void GetVariableInstruction::print(std::ostream &os) const
{
    os << "getv " << m_Name << "(" << m_Offset << ")"
       << " A" << m_ArgumentRegister;
}

// Put Instructions

PutInstruction::PutInstruction(const std::string &name, size_t argumentRegister)
    : m_Name(name),
      m_ArgumentRegister(argumentRegister)
{
}

PutConstantInstruction::PutConstantInstruction(const std::string &name, size_t argumentRegister)
    : PutInstruction(name, argumentRegister)
{
}

std::shared_ptr<Instruction> PutConstantInstruction::clone(void)
{
    return std::make_shared<PutConstantInstruction>(m_Name, m_ArgumentRegister);
}

void PutConstantInstruction::execute(WAMState &state)
{
    state.fillRegister(std::make_shared<ConstantWord>(m_Name), m_ArgumentRegister);
}

void PutConstantInstruction::print(std::ostream &os) const
{
    os << "put-constant " << m_Name << " A" << m_ArgumentRegister;
}

PutVariableInstruction::PutVariableInstruction(const std::string &name,
                                               size_t argumentRegister, size_t offset)
    : PutInstruction(name, argumentRegister),
      m_Offset(offset)
{
}

std::shared_ptr<Instruction> PutVariableInstruction::clone(void)
{
    return std::make_shared<PutVariableInstruction>(m_Name, m_ArgumentRegister, m_Offset);
}

void PutVariableInstruction::execute(WAMState &state)
{
    std::shared_ptr<ChoicePoint> cp = state.stackAt(state.EReg());
    std::shared_ptr<Word> word = cp->m_Variables[m_Offset]->dereference();
    if (word->tag() == TAG::VARIABLE)
    {
        std::shared_ptr<VariableWord> vw = std::make_shared<VariableWord>(&cp->m_Variables[m_Offset], "", true);
        state.fillRegister(vw, m_ArgumentRegister);
    }
    else
        state.fillRegister(word->clone(), m_ArgumentRegister);
}

void PutVariableInstruction::print(std::ostream &os) const
{
    os << "putv " << m_Name << "(" << m_Offset << ")"
       << " A" << m_ArgumentRegister;
}

// TODO: name is redundant here
PutListInstruction::PutListInstruction(const std::string &name, size_t ArgumentRegister)
    : PutInstruction(name, ArgumentRegister)
{
}

std::shared_ptr<Instruction> PutListInstruction::clone(void)
{
    return std::make_shared<PutListInstruction>(m_Name, m_ArgumentRegister);
}

void PutListInstruction::execute(WAMState &state)
{
    state.fillRegister(std::make_shared<ListWord>(state.HReg(), state.m_Heap), m_ArgumentRegister);
    state.setWriteMode();
}

void PutListInstruction::print(std::ostream &os) const
{
    os << "put-list A" << m_ArgumentRegister;
}

PutStructureInstruction::PutStructureInstruction(const std::string &name, size_t argumentRegister, size_t arity)
    : PutInstruction(name, argumentRegister),
      m_Arity(arity)
{
}

std::shared_ptr<Instruction> PutStructureInstruction::clone(void)
{
    return std::make_shared<PutStructureInstruction>(m_Name, m_ArgumentRegister, m_Arity);
}

void PutStructureInstruction::execute(WAMState &state)
{
    state.fillRegister(std::make_shared<StructurePointerWord>(state.HReg(), state.m_Heap), m_ArgumentRegister);
    state.heapPush(std::make_shared<StructureWord>(m_Name, m_Arity, state.m_Heap, state.HReg()));
    state.setWriteMode();
}

void PutStructureInstruction::print(std::ostream &os) const
{
    os << "put-structure " << m_Name << "/" << m_Arity << " A" << m_ArgumentRegister;
}

size_t m_Arity;

// Unify Instructions

UnifyInstruction::UnifyInstruction(const std::string &name)
    : m_Name(name)
{
}

UnifyConstantInstruction::UnifyConstantInstruction(const std::string &name)
    : UnifyInstruction(name)
{
}

std::shared_ptr<Instruction> UnifyConstantInstruction::clone(void)
{
    return std::make_shared<UnifyConstantInstruction>(m_Name);
}

void UnifyConstantInstruction::execute(WAMState &state)
{
    if (!state.readMode())
    {
        state.heapPush(std::make_shared<ConstantWord>(m_Name));
        return;
    }

    std::shared_ptr<Word> w = state.heapAt(state.SPReg())->dereference();
    state.m_StructurePointer++;

    if (w->tag() == VARIABLE)
    {
        std::shared_ptr<VariableWord> vw = std::static_pointer_cast<VariableWord>(w);
        state.trailPush(vw);
        // TODO: check
        vw->bind(std::make_shared<ConstantWord>(m_Name));
        //*vw->ref() = std::make_shared<ConstantWord>(m_Name);
    }

    else if (w->tag() == CONSTANT)
    {
        std::shared_ptr<ConstantWord> cw = std::static_pointer_cast<ConstantWord>(w);
        std::shared_ptr<ConstantWord> c = std::make_shared<ConstantWord>(m_Name);
        if (!cw->compareToConst(c))
        {
            fail(state);
        }
    }

    else
    {
        fail(state);
    }
}

void UnifyConstantInstruction::print(std::ostream &os) const
{
    os << "unify-constant " << m_Name;
}

UnifyVariableInstruction::UnifyVariableInstruction(const std::string &name, size_t offset)
    : UnifyInstruction(name),
      m_Offset(offset)
{
}

std::shared_ptr<Instruction> UnifyVariableInstruction::clone(void)
{
    return std::make_shared<UnifyVariableInstruction>(m_Name, m_Offset);
}

void UnifyVariableInstruction::execute(WAMState &state)
{
    std::shared_ptr<Word> w = state.stackAt(state.EReg())->m_Variables[m_Offset]->dereference();
    if (!state.readMode())
    {
        state.heapPush(w);
        return;
    }

    std::shared_ptr<Word> sp = state.heapAt(state.SPReg())->dereference();
    // unify (similiar to getv)
    clearPDL(state, w, sp);
    state.m_StructurePointer++;
}

void UnifyVariableInstruction::print(std::ostream &os) const
{
    os << "unifyv " << m_Name << "(" << m_Offset << ")";
}

std::shared_ptr<Instruction> CutInstruction::clone(void)
{
    return std::make_shared<CutInstruction>();
}

void CutInstruction::execute(WAMState &state)
{
    state.m_BacktrackRegister = state.stackAt(state.EReg())->m_BB;
}

void CutInstruction::print(std::ostream &os) const
{
    os << "cut (!)";
}
