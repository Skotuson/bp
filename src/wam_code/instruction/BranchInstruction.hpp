#pragma once

#include "Instruction.hpp"

struct BranchInstruction : public Instruction
{
    BranchInstruction(const std::string &label, size_t address = 0);

    /**
     * @brief Sets the target address of a branch instruction
     * @param address Address to be set
     */
    void setAddress(size_t address);

    /**
     * @return Returns the name of the target label tied to the branch instruction
     */
    std::string label(void);

protected:
    std::string m_Label;
    size_t m_Address = 0;
};