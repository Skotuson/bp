#ifndef CHOICEPOINT_H
#define CHOICEPOINT_H

#include "ArgumentRegisters.hpp"

struct ChoicePoint
{
    ArgumentRegisters m_ArgumentRegisters;
    size_t m_BCP; // Backtrack Continuation Pointer
    size_t m_BCE; // Backtrack Continuation Environment
    size_t m_BB;  // Backtrack B (register)
    size_t m_FA;  // Failure Address
};

#endif // CHOICEPOINT_H