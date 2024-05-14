#pragma once

#include "../Instruction.hpp"

struct GetList : public Instruction
{
    GetList(size_t argumentRegister);
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;

private:
    // Argument register to dereference
    size_t m_ArgumentRegister;
};