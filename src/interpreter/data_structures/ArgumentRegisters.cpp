#include "ArgumentRegisters.hpp"

ArgumentRegisters::ArgumentRegisters(const ArgumentRegisters &argReg)
{
    // TODO: this won't copy possible gaps in registers
    for (const auto &arg : argReg.m_ArgumentRegisters)
    {
        m_ArgumentRegisters.push_back(arg->clone());
    }
}

void ArgumentRegisters::fillRegister(Word *word, size_t reg)
{
    //if (m_ArgumentRegisters.capacity() < reg)
    //{
    //    m_ArgumentRegisters.reserve(reg * 2);
    //}
    m_ArgumentRegisters.insert(m_ArgumentRegisters.begin() + (reg - 1), word);
}

Word *ArgumentRegisters::dereferenceRegister(size_t reg) const
{
    if (reg > m_ArgumentRegisters.size())
        return nullptr;
    return m_ArgumentRegisters[reg - 1];
}

std::ostream &operator<<(std::ostream &os, const ArgumentRegisters &argReg)
{
    for (size_t i = 1; i <= argReg.m_ArgumentRegisters.size(); i++)
    {
        os << "A" << i << ": ";
        argReg.dereferenceRegister(i)->print(os);
        os << std::endl;
    }
    return os;
}