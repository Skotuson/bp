#pragma once

#include "../BranchInstruction.hpp"

struct RetryMeElse : public BranchInstruction
{
    RetryMeElse(const std::string &label, size_t address = 0);
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
};