#include "WAMState.hpp"

void WAMState::fillRegister(Word *word, size_t reg)
{
    if(m_ArgumentRegisters.capacity() < reg)
        m_ArgumentRegisters.reserve(reg*2);
    m_ArgumentRegisters[reg - 1] = word;
}