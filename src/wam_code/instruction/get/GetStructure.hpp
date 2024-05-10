#pragma once

#include "../Instruction.hpp"

struct GetStructure : public Instruction
{
    GetStructure(const std::string &name, size_t argumentRegister, size_t arity);
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;

    std::string m_Name;
    size_t m_ArgumentRegister;
    size_t m_Arity;
};