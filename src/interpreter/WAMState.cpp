#include "WAMState.hpp"

void WAMState::fillRegister(Word *word, size_t reg)
{
    m_ArgumentRegisters.fillRegister(word, reg);
}

size_t WAMState::SReg(void)
{
    return m_Stack.size();
}

size_t WAMState::TRReg(void)
{
    return m_Trail.size();
}

void WAMState::stackPush(ChoicePoint *cp)
{
    m_Stack.push_back(cp);
}

void WAMState::stackPop(void)
{
    m_Stack.pop_back();
}

ChoicePoint *WAMState::stackTop(void)
{
    if (m_Stack.size())
    {
        return m_Stack[m_Stack.size() - 1];
    }

    return nullptr;
}