#pragma once

#include "../../interpreter/WAMState.hpp"

#include <ostream>
#include <string>
#include <vector>

struct Instruction
{
    virtual std::shared_ptr<Instruction> clone(void) = 0;
    virtual ~Instruction(void) = default;
    /**
     * @brief Execute the instruction
     * @param state Reference to WAMState class
     */
    virtual void execute(WAMState &state) = 0;
    virtual void print(std::ostream &os) const = 0;

    /**
     * @brief Initiates the fail sequence
     * @param state Reference to WAMState class
     */
    static void fail(WAMState &state);

    /**
     * @brief Executes the Clear PDL operation
     * @param state Reference to WAMState class
     * @param X Memory Word
     * @param Y Memory Word
     */
    static void clearPDL(WAMState &state, std::shared_ptr<Word> X, std::shared_ptr<Word> Y);
    static const std::vector<std::vector<size_t>> m_ClearPDLTable;

    friend std::ostream &operator<<(std::ostream &os, const Instruction &instr);
};