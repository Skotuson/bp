#pragma once

#include "../Instruction.hpp"

struct AllocateInstruction : public Instruction
{
    AllocateInstruction(size_t n);
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
    size_t m_N;
};