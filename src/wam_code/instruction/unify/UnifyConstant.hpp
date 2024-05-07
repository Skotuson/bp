#pragma once

#include "../Instruction.hpp"

struct UnifyConstant : public Instruction
{
    UnifyConstant(const std::string &name);
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;

    std::string m_Name;
};