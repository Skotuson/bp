#pragma once

#include "Instruction.hpp"

struct ReturnInstruction : public Instruction
{
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
};