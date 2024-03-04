#include "Instruction.hpp"

#include <iostream>

// Indexing Instructions

Instruction *MarkInstruction::clone(void)
{
    return new MarkInstruction();
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
    ChoicePoint *cp = state.stackTop();
    ChoicePoint *ncp;
    if (cp)
    {
        ncp = new ChoicePoint(cp->m_ArgumentRegisters,
                              state.m_EnvironmentRegister,
                              state.m_ContinuationPointer,
                              state.m_BacktrackRegister,
                              state.m_ProgramCounter);
    }
    else
        ncp = new ChoicePoint(state.m_ArgumentRegisters,
                              state.m_EnvironmentRegister,
                              state.m_ContinuationPointer,
                              state.m_BacktrackRegister,
                              state.m_ProgramCounter);
    state.stackPush(ncp);
    state.m_BacktrackRegister = state.SReg();
    std::cout << state << std::endl;
}

void MarkInstruction::print(std::ostream &os)
{
    os << "mark";
}

RetryMeElseInstruction::RetryMeElseInstruction(const std::string &label, size_t address)
    : BranchInstruction(label, address)
{
}

Instruction *RetryMeElseInstruction::clone(void)
{
    return new RetryMeElseInstruction(m_Label, m_Address);
}

void RetryMeElseInstruction::execute(WAMState &state)
{
    ChoicePoint *cp = state.stackTop();
    if (cp)
    {
        cp->m_FA = m_Address;
    }
}

void RetryMeElseInstruction::print(std::ostream &os)
{
    os << "retry-me-else " << m_Label << "[" << m_Address << "]";
}

Instruction *BacktrackInstruction::clone(void)
{
    return new BacktrackInstruction();
}

void BacktrackInstruction::execute(WAMState &state)
{
    ChoicePoint *cp = state.getChoicePoint(state.m_BacktrackRegister);
    if (cp)
    {
        state.m_BacktrackRegister = cp->m_BB;
    }

    Instruction *fail = new FailInstruction();
    fail->execute(state);
    delete fail;

    state.stackPop(); // Discard the choice point (last clause in the chain failed)
}

void BacktrackInstruction::print(std::ostream &os)
{
    os << "backtrack";
}

Instruction *FailInstruction::clone(void)
{
    return new FailInstruction();
}

void FailInstruction::execute(WAMState &state)
{
    ChoicePoint *cp = state.getChoicePoint(state.m_BacktrackRegister);
    if (cp)
    {
        state.m_ProgramCounter = cp->m_FA;
    }
    //TODO: Experimental (check when choice point stack is empty)
    else if (state.m_BacktrackRegister == UNSET_REG)
        state.m_FailFlag = true;
}

void FailInstruction::print(std::ostream &os)
{
    os << "__fail__";
}

// Procedural Instructions
AllocateInstruction::AllocateInstruction(size_t n)
    : m_N(n)
{
}

Instruction *AllocateInstruction::clone(void)
{
    return new AllocateInstruction(m_N);
}

void AllocateInstruction::execute(WAMState &state)
{
    state.m_EnvironmentRegister = state.m_BacktrackRegister;
}

void AllocateInstruction::print(std::ostream &os)
{
    os << "allocate " << m_N;
}

CallInstruction::CallInstruction(const std::string &label, size_t address)
    : BranchInstruction(label, address)
{
}

Instruction *CallInstruction::clone(void)
{
    return new CallInstruction(m_Label, m_Address);
}

void CallInstruction::execute(WAMState &state)
{
    // Program counter already points to another instruction
    state.m_ContinuationPointer = state.m_ProgramCounter;
    state.m_ProgramCounter = m_Address;
}

void CallInstruction::print(std::ostream &os)
{
    os << "call " + m_Label << "[" << m_Address << "]";
}

Instruction *ReturnInstruction::clone(void)
{
    return new ReturnInstruction();
}

void ReturnInstruction::execute(WAMState &state)
{
    ChoicePoint *cp = state.getChoicePoint(state.m_EnvironmentRegister);
    if (cp)
    {
        state.m_ProgramCounter = cp->m_BCP;
        state.m_EnvironmentRegister = cp->m_BCE;
    }
    std::cout << state << std::endl;
}

void ReturnInstruction::print(std::ostream &os)
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

Instruction *GetConstantInstruction::clone(void)
{
    return new GetConstantInstruction(m_Name, m_ArgumentRegister);
}

void GetConstantInstruction::execute(WAMState &state)
{
    Word *reg = state.m_ArgumentRegisters.dereferenceRegister(m_ArgumentRegister);
    ConstantWord *cword = new ConstantWord(m_Name);
    if (!reg || !reg->compareToConst(cword))
    {
        // TODO: Make a singleton object maybe?
        Instruction *fail = new FailInstruction();
        fail->execute(state);
        delete fail;
    }
    delete cword;
}

void GetConstantInstruction::print(std::ostream &os)
{
    os << "get-constant " << m_Name << " A" << m_ArgumentRegister;
}

GetStructureInstruction::GetStructureInstruction(const std::string &name, size_t argumentRegister)
    : GetInstruction(name, argumentRegister)
{
}

Instruction *GetStructureInstruction::clone(void)
{
    return new GetStructureInstruction(m_Name, m_ArgumentRegister);
}

void GetStructureInstruction::execute(WAMState &state)
{
}

void GetStructureInstruction::print(std::ostream &os)
{
    os << "get-structure " << m_Name << " A" << m_ArgumentRegister;
}

GetVariableInstruction::GetVariableInstruction(const std::string &name, size_t argumentRegister)
    : GetInstruction(name, argumentRegister)
{
}

Instruction *GetVariableInstruction::clone(void)
{
    return new GetVariableInstruction(m_Name, m_ArgumentRegister);
}

void GetVariableInstruction::execute(WAMState &state)
{
}

void GetVariableInstruction::print(std::ostream &os)
{
    os << "getv " << m_Name << " A" << m_ArgumentRegister;
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

Instruction *PutConstantInstruction::clone(void)
{
    return new PutConstantInstruction(m_Name, m_ArgumentRegister);
}

void PutConstantInstruction::execute(WAMState &state)
{
    state.fillRegister(new ConstantWord(m_Name), m_ArgumentRegister);
}

void PutConstantInstruction::print(std::ostream &os)
{
    os << "put-constant " << m_Name << " A" << m_ArgumentRegister;
}

PutVariableInstruction::PutVariableInstruction(const std::string &name, size_t argumentRegister)
    : PutInstruction(name, argumentRegister)
{
}

Instruction *PutVariableInstruction::clone(void)
{
    return new PutVariableInstruction(m_Name, m_ArgumentRegister);
}

void PutVariableInstruction::execute(WAMState &state)
{
}

void PutVariableInstruction::print(std::ostream &os)
{
    os << "putv " << m_Name << " A" << m_ArgumentRegister;
}

// Unify Instructions

UnifyInstruction::UnifyInstruction(const std::string &name)
    : m_Name(name)
{
}

UnifyConstantInstruction::UnifyConstantInstruction(const std::string &name)
    : UnifyInstruction(name)
{
}

Instruction *UnifyConstantInstruction::clone(void)
{
    return new UnifyConstantInstruction(m_Name);
}

void UnifyConstantInstruction::execute(WAMState &state)
{
}

void UnifyConstantInstruction::print(std::ostream &os)
{
    os << "unify-constant " << m_Name;
}

UnifyVariableInstruction::UnifyVariableInstruction(const std::string &name)
    : UnifyInstruction(name)
{
}

Instruction *UnifyVariableInstruction::clone(void)
{
    return new UnifyVariableInstruction(m_Name);
}

void UnifyVariableInstruction::execute(WAMState &state)
{
}

void UnifyVariableInstruction::print(std::ostream &os)
{
    os << "unifyv " << m_Name;
}