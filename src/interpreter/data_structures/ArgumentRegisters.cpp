#include "ArgumentRegisters.hpp"

ArgumentRegisters::~ArgumentRegisters(void)
{
    for (Word *word : m_ArgumentRegisters)
    {
        delete word;
    }
}

ArgumentRegisters::ArgumentRegisters(const ArgumentRegisters &argReg)
{
    // TODO: this won't copy possible gaps in registers
    for (const auto &arg : argReg.m_ArgumentRegisters)
    {
        m_ArgumentRegisters.push_back(arg->clone());
    }
}

ArgumentRegisters &ArgumentRegisters::operator=(const ArgumentRegisters &argReg)
{
    if (&argReg == this)
        return *this;
    for (const auto &arg : argReg.m_ArgumentRegisters)
    {
        m_ArgumentRegisters.push_back(arg->clone());
    }

    return *this;
}

void ArgumentRegisters::fillRegister(Word *word, size_t reg)
{
    if (reg > m_ArgumentRegisters.size())
    {
        while (m_ArgumentRegisters.size() < reg - 1)
            m_ArgumentRegisters.push_back(nullptr);
        m_ArgumentRegisters.push_back(word);
    }
    else
    {
        auto old = m_ArgumentRegisters[reg - 1];
        m_ArgumentRegisters[reg - 1] = word;
        delete old;
    }
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
        if (i != 1)
            os << std::endl;
        os << "A" << i << ": ";
        auto reg = argReg.dereferenceRegister(i);
        if (reg)
            reg->print(os);
        else
            os << "EMPTY";
    }
    return os;
}