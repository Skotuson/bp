#include "Instruction.hpp"

// Indexing Instructions

void MarkInstruction::execute(WAMState &state)
{
}

void MarkInstruction::print(std::ostream &os)
{
    os << "mark";
}

RetryMeElseInstruction::RetryMeElseInstruction(const std::string &label)
    : m_Label(label)
{
}

void RetryMeElseInstruction::execute(WAMState &state)
{
}

void RetryMeElseInstruction::print(std::ostream &os)
{
    os << "retry-me-else " << m_Label;
}

void BacktrackInstruction::execute(WAMState &state)
{
}

void BacktrackInstruction::print(std::ostream &os)
{
    os << "backtrack";
}

// Procedural Instructions
CallInstruction::CallInstruction(const std::string &label)
    : m_Label(label)
{
}

void CallInstruction::execute(WAMState &state)
{
}

void CallInstruction::print(std::ostream &os)
{
    os << "call " + m_Label;
}

void ReturnInstruction::execute(WAMState &state)
{
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

void GetConstantInstruction::execute(WAMState &state)
{
}

void GetConstantInstruction::print(std::ostream &os)
{
    os << "get-constant " << m_Name << " A" << m_ArgumentRegister;
}

GetStructureInstruction::GetStructureInstruction(const std::string &name, size_t argumentRegister)
    : GetInstruction(name, argumentRegister)
{
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

void PutConstantInstruction::execute(WAMState &state)
{
}

void PutConstantInstruction::print(std::ostream &os)
{
    os << "put-constant " << m_Name << " A" << m_ArgumentRegister;
}

PutVariableInstruction::PutVariableInstruction(const std::string &name, size_t argumentRegister)
    : PutInstruction(name, argumentRegister)
{
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

void UnifyVariableInstruction::execute(WAMState &state)
{
}

void UnifyVariableInstruction::print(std::ostream &os)
{
    os << "unifyv " << m_Name;
}