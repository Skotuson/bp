#include "Instruction.hpp"

// Indexing Instructions
void MarkInstruction::print(std::ostream &os)
{
    os << "mark";
}

RetryMeElseInstruction::RetryMeElseInstruction(const std::string &label)
    : m_Label(label)
{
}

void RetryMeElseInstruction::print(std::ostream &os)
{
    os << "retry-me-else" << m_Label;
}

// Procedural Instructions
void CallInstruction::print(std::ostream &os)
{
    os << "call";
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

void GetConstantInstruction::print(std::ostream &os)
{
    os << "get-constant " << m_Name << " A" << m_ArgumentRegister;
}

GetStructureInstruction::GetStructureInstruction(const std::string &name, size_t argumentRegister)
    : GetInstruction(name, argumentRegister)
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

void PutConstantInstruction::print(std::ostream &os)
{
    os << "put-constant " << m_Name << " A" << m_ArgumentRegister;
}

PutVariableInstruction::PutVariableInstruction(const std::string &name, size_t argumentRegister)
    : PutInstruction(name, argumentRegister)
{
}

void PutVariableInstruction::print(std::ostream &os)
{
    os << "putv" << m_Name << " A" << m_ArgumentRegister;
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

void UnifyConstantInstruction::print(std::ostream &os)
{
    os << "unify-constant " << m_Name;
}

UnifyVariableInstruction::UnifyVariableInstruction(const std::string &name)
    : UnifyInstruction(name)
{
}

void UnifyVariableInstruction::print(std::ostream &os)
{
    os << "unifyv" << m_Name;
}