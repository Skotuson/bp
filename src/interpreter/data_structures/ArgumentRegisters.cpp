#include "ArgumentRegisters.hpp"

ArgumentRegisters::ArgumentRegisters(const ArgumentRegisters &argReg)
{
    for (const auto &arg : argReg.m_ArgumentRegisters)
    {
        m_ArgumentRegisters.push_back(arg->clone());
    }
}

void ArgumentRegisters::fillRegister(Word *word, size_t reg)
{
    if (m_ArgumentRegisters.capacity() < reg)
    {
        m_ArgumentRegisters.reserve(reg * 2);
    }
    m_ArgumentRegisters[reg - 1] = word;
}

Word *ArgumentRegisters::dereferenceRegister(size_t reg)
{
    if (reg > m_ArgumentRegisters.size())
        return nullptr;
    return m_ArgumentRegisters[reg - 1];
}