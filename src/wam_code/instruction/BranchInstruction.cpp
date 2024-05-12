#include "BranchInstruction.hpp"

BranchInstruction::BranchInstruction(const std::string &label, size_t address)
    : m_Label(label),
      m_Address(address)
{
}

void BranchInstruction::setAddress(size_t address)
{
    m_Address = address;
}

std::string BranchInstruction::label(void)
{
    return m_Label;
}