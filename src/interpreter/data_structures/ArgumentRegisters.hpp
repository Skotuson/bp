#ifndef ARGUMENT_REGISTERS_H
#define ARGUMENT_REGISTERS_H

#include "Word.hpp"

#include <vector>

class ArgumentRegisters
{
public:
    ArgumentRegisters(void) = default;
    ArgumentRegisters(const ArgumentRegisters &argReg);

    void fillRegister(Word *word, size_t reg);
    Word *dereferenceRegister(size_t reg);

private:
    std::vector<Word *> m_ArgumentRegisters;
};

#endif