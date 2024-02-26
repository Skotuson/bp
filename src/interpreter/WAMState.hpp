#ifndef WAMSTATE_H
#define WAMSTATE_H

#include "data_structures/Word.hpp"
#include "data_structures/ChoicePoint.hpp"
#include "data_structures/ArgumentRegisters.hpp"

#include <stack>
#include <vector>
#include <cstdlib>

struct WAMState
{
    void fillRegister(Word *word, size_t reg);

    size_t m_ProgramCounter = 0;
    size_t m_ContinuationPointer = 0;

    std::stack<ChoicePoint> m_Stack;
    ArgumentRegisters m_ArgumentRegisters;

    bool m_ModeFlag = false;
};

#endif // WAMSTATE_H