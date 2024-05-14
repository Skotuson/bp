#pragma once

#include "../Instruction.hpp"

struct GetConstant : public Instruction
{
    GetConstant(const std::string &name, size_t argumentRegister);
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
private:
    // Constant value
    std::string m_Name;
    // Register to dereference
    size_t m_ArgumentRegister;
};