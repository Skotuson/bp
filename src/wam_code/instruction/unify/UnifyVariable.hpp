#pragma once

#include "../Instruction.hpp"

struct UnifyVariable : public Instruction
{
    UnifyVariable(const std::string &name, size_t offset);
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;

    std::string m_Name;
    size_t m_Offset;
};