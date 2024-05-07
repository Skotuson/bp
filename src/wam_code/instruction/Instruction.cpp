#include "Instruction.hpp"

#include "../../word/ListWord.hpp"

#include <iostream>
#include <cassert>
#include <memory>

const std::vector<std::vector<size_t>> Instruction::m_ClearPDLTable = {
    {1, 1, 1, 1, 1},
    {2, 3, 5, 5, 5},
    {2, 4, 6, 0, 0},
    {2, 4, 0, 7, 0},
    {2, 4, 0, 0, 8}};

void Instruction::fail(WAMState &state)
{
    std::shared_ptr<FailInstruction> fail = std::make_shared<FailInstruction>();
    fail->execute(state);
}

void Instruction::clearPDL(WAMState &state, std::shared_ptr<Word> X, std::shared_ptr<Word> Y)
{
    // Empty the PDL
    while (!state.pdlEmpty())
    {
        state.pdlPop();
    }

    while (42)
    {
        size_t branch = m_ClearPDLTable[X->tag()][Y->tag()];
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
            y->bind(X);
        }

        // X is not a variable, Y is an unbound variable
        else if (branch == 4)
        {
            std::shared_ptr<VariableWord> vw = std::static_pointer_cast<VariableWord>(Y);
            // Trail
            state.trailPush(vw);
            vw->bind(X->clone());
        }

        // Y is a constant, X is an unbound variable
        else if (branch == 5)
        {
            std::shared_ptr<VariableWord> vw = std::static_pointer_cast<VariableWord>(X);
            // Trail
            state.trailPush(vw);
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
        vw->bind(std::make_shared<ConstantWord>(m_Name));
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