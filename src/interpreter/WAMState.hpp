#ifndef WAMSTATE_H
#define WAMSTATE_H

#include "data_structures/Word.hpp"
#include "data_structures/ChoicePoint.hpp"

#include <stack>
#include <vector>
#include <cstdlib>

struct WAMState
{
    void fillRegister(Word *word, size_t reg);

    std::stack<ChoicePoint> m_Stack;
    std::vector<Word *> m_ArgumentRegisters;
    size_t m_ProgramCounter = 0;
};

#endif // WAMSTATE_H