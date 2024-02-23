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
    os << "get-constant " << m_Name;
}

GetVariableInstruction::GetVariableInstruction(const std::string &name, size_t argumentRegister)
    : GetInstruction(name, argumentRegister)
{
}

void GetVariableInstruction::print(std::ostream &os)
{
    os << "getv" << m_Name;
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
    os << "put-constant " << m_Name;
}

PutVariableInstruction::PutVariableInstruction(const std::string &name, size_t argumentRegister)
    : PutInstruction(name, argumentRegister)
{
}

void PutVariableInstruction::print(std::ostream &os)
{
    os << "putv" << m_Name;
}