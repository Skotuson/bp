#pragma once

#include "../Instruction.hpp"

struct GetVariable : public Instruction
{
    GetVariable(const std::string &name, size_t argumentRegister, size_t offset);
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;

private:
    // Variable name
    std::string m_Name;
    // Argument register to dereference
    size_t m_ArgumentRegister;
    // Variable offset in an environment
    size_t m_Offset;
};