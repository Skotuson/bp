#pragma once

#include "Instruction.hpp"

struct BranchInstruction : public Instruction
{
    BranchInstruction(const std::string &label, size_t address = 0);
    void setAddress(size_t address);
    std::string label(void);

protected:
    std::string m_Label;
    size_t m_Address = 0;
};