#pragma once

#include "../Instruction.hpp"

struct GetConstant : public Instruction
{
    GetConstant(const std::string &name, size_t argumentRegister);
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;

    std::string m_Name;
    size_t m_ArgumentRegister;
};