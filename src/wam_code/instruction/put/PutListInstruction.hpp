#pragma once

#include "../Instruction.hpp"

struct PutListInstruction : public Instruction
{
    PutListInstruction(size_t argumentRegister);
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;

    size_t m_ArgumentRegister;
};