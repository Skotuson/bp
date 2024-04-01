#pragma once

#include "word/Word.hpp"

#include <vector>

class ArgumentRegisters
{
public:
    ArgumentRegisters(void) = default;
    ~ArgumentRegisters(void);
    ArgumentRegisters(const ArgumentRegisters &argReg);
    ArgumentRegisters &operator=(const ArgumentRegisters &argReg);

    void fillRegister(Word *word, size_t reg);
    Word *dereferenceRegister(size_t reg) const;

    friend std::ostream &operator<<(std::ostream &os, const ArgumentRegisters &argReg);

private:
    std::vector<Word *> m_ArgumentRegisters;
};