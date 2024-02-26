#include "ArgumentRegisters.hpp"

void ArgumentRegisters::fillRegister(Word *word, size_t reg)
{
    if (m_ArgumentRegisters.capacity() < reg)
        m_ArgumentRegisters.reserve(reg * 2);
    m_ArgumentRegisters[reg - 1] = word;
}

Word *ArgumentRegisters::dereferenceRegister(size_t reg)
{
    if (reg > m_ArgumentRegisters.size())
        return nullptr;
    return m_ArgumentRegisters[reg - 1];
}