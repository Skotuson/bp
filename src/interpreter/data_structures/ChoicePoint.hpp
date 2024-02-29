#ifndef CHOICEPOINT_H
#define CHOICEPOINT_H

#include "ArgumentRegisters.hpp"

struct ChoicePoint
{
    ChoicePoint(const ArgumentRegisters &argReg, size_t bcp, size_t bce, size_t bb, size_t fa);

    friend std::ostream &operator<<(std::ostream &os, const ChoicePoint &cp);

    ArgumentRegisters m_ArgumentRegisters;
    size_t m_BCP; // Backtrack Continuation Pointer
    size_t m_BCE; // Backtrack Continuation Environment
    size_t m_BB;  // Backtrack B (register)
    size_t m_FA;  // Failure Address
};

#endif // CHOICEPOINT_H