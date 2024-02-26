#ifndef ARGUMENT_REGISTERS_H
#define ARGUMENT_REGISTERS_H

#include "Word.hpp"

#include <vector>

class ArgumentRegisters
{
public:
    void fillRegister(Word *word, size_t reg);
    Word *dereferenceRegister(size_t reg);
private:
    std::vector<Word *> m_ArgumentRegisters;
};

#endif