#pragma once

#include "../Instruction.hpp"

struct PutList : public Instruction
{
    PutList(size_t argumentRegister);
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;

private:
    size_t m_ArgumentRegister;
};