#pragma once

#include "../../word/Word.hpp"

#include <vector>
#include <memory>

class ArgumentRegisters
{
public:
    ArgumentRegisters(void) = default;
    ArgumentRegisters(const ArgumentRegisters &argReg);
    ArgumentRegisters &operator=(const ArgumentRegisters &argReg);

    /**
     * @brief Load a specific register
     * @param word Word to load into a register
     * @param reg Number of register to load. As the registers are labeled from 1, this method accounts for it
     */
    void fillRegister(std::shared_ptr<Word> word, size_t reg);

    /**
     * @brief Dereferences a specific register. If a reference is encountered, it is chased until something other than a reference is encountered.
     * @param reg Number of register to dereference. As the registers are labeled from 1, this method accounts for it
     */
    std::shared_ptr<Word> dereferenceRegister(size_t reg) const;

    friend std::ostream &operator<<(std::ostream &os, const ArgumentRegisters &argReg);

private:
    std::vector<std::shared_ptr<Word>> m_ArgumentRegisters;
};