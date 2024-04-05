#pragma once

#include "word/Word.hpp"

#include <vector>
#include <memory>

class ArgumentRegisters
{
public:
    ArgumentRegisters(void) = default;
    ArgumentRegisters(const ArgumentRegisters &argReg);
    ArgumentRegisters &operator=(const ArgumentRegisters &argReg);

    void fillRegister(std::shared_ptr<Word> word, size_t reg);
    std::shared_ptr<Word> dereferenceRegister(size_t reg) const;

    friend std::ostream &operator<<(std::ostream &os, const ArgumentRegisters &argReg);

private:
    std::vector<std::shared_ptr<Word>> m_ArgumentRegisters;
};