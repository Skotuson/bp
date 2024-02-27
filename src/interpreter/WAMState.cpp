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