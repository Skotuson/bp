#pragma once

#include "ArgumentRegisters.hpp"
#include "../../word/Word.hpp"
#include <vector>

struct ChoicePoint
{
    ChoicePoint(const ArgumentRegisters &argReg, size_t bce, size_t bcp, size_t bb, size_t btr, size_t bh, size_t fa);

    friend std::ostream &operator<<(std::ostream &os, const ChoicePoint &cp);

    ArgumentRegisters m_ArgumentRegisters;
    size_t m_BCE; // Backtrack Continuation Environment
    size_t m_BCP; // Backtrack Continuation Pointer
    size_t m_BB;  // Backtrack B (register)
    size_t m_BTR; // Backtrack Trail Top
    size_t m_BH; // Backtrack Heap Top
    size_t m_FA;  // Failure Address
    std::vector<std::shared_ptr<Word>> m_Variables;
};