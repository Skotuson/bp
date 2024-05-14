#pragma once

#include "../Instruction.hpp"

struct Allocate : public Instruction
{
    Allocate(size_t n);
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;

    /**
     * @brief Sets the number of local variables to allocate in the environment
     * @param n Number of local variables to allocate
    */
    void setN(size_t n);

private:
    size_t m_N;
};