#include "Instruction.hpp"

// Indexing Instructions
void MarkInstruction::print(std::ostream &os)
{
    os << "mark";
}

void RetryMeElseInstruction::print(std::ostream &os)
{
    os << "retry-me-else";
}

// Procedural Instructions
void CallInstruction::print(std::ostream &os)
{
    os << "call";
}

// Get Instructions
void GetConstantInstruction::print(std::ostream &os)
{
    os << "get-constant";
}

void GetVariableInstruction::print(std::ostream &os)
{
    os << "getv";
}