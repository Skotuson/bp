#ifndef WAMSTATE_H
#define WAMSTATE_H

#include "data_structures/ChoicePoint.hpp"

#include <stack>
#include <cstdlib>

struct WAMState
{
    std::stack<ChoicePoint> m_Stack;
    size_t m_ProgramCounter = 0;
};

#endif // WAMSTATE_H