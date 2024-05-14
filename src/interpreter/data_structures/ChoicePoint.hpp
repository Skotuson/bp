#pragma once

#include "ArgumentRegisters.hpp"
#include "../../word/Word.hpp"
#include <vector>

struct ChoicePoint
{
    ChoicePoint(const ArgumentRegisters &argReg, size_t bce, size_t bcp, size_t bb, size_t btr, size_t bh, size_t fa);
    
    friend std::ostream &operator<<(std::ostream &os, const ChoicePoint &cp);
    // Copy of the argument registers
    ArgumentRegisters m_ArgumentRegisters;
    // Backtrack Continuation Environment
    size_t m_BCE;
    // Backtrack Continuation Pointer
    size_t m_BCP;
    // Backtrack B (register)
    size_t m_BB;
    // Backtrack Trail Top
    size_t m_BTR;
    // Backtrack Heap Top
    size_t m_BH;
    // Failure Address
    size_t m_FA;
    // Variables for the local environment
    std::vector<std::shared_ptr<Word>> m_Variables;
};