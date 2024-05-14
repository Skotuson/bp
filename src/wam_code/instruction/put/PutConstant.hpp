#pragma once

#include "../Instruction.hpp"

struct PutConstant : public Instruction
{
    PutConstant(const std::string &name, size_t argumentRegister);
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;

private:
    std::string m_Name;
    size_t m_ArgumentRegister;
};