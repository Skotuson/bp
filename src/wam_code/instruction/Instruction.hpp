#pragma once

#include "../../interpreter/WAMState.hpp"

#include <ostream>
#include <string>

struct Instruction
{
    virtual std::shared_ptr<Instruction> clone(void) = 0;
    virtual ~Instruction(void) = default;
    virtual void execute(WAMState &state) = 0;
    virtual void print(std::ostream &os) const = 0;

    static void fail(WAMState &state);
    static void clearPDL(WAMState &state, std::shared_ptr<Word> X, std::shared_ptr<Word> Y);

    static const std::vector<std::vector<size_t>> m_ClearPDLTable;

    friend std::ostream &operator<<(std::ostream &os, const Instruction &instr);
};

struct FailInstruction : public Instruction
{
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
};