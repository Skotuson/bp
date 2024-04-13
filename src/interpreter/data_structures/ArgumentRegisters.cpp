#include "ArgumentRegisters.hpp"

ArgumentRegisters::ArgumentRegisters(const ArgumentRegisters &argReg)
{
    std::vector<std::shared_ptr<Word>> cpy;
    for (const auto &arg : argReg.m_ArgumentRegisters)
    {
        if (arg)
        {
            cpy.push_back(arg->clone());
        }
    }
    m_ArgumentRegisters = cpy;
}

ArgumentRegisters &ArgumentRegisters::operator=(const ArgumentRegisters &argReg)
{
    if (&argReg == this)
        return *this;

    std::vector<std::shared_ptr<Word>> cpy;

    for (const auto &arg : argReg.m_ArgumentRegisters)
    {
        cpy.push_back(arg->clone());
    }

    m_ArgumentRegisters = cpy;
    return *this;
}

void ArgumentRegisters::fillRegister(std::shared_ptr<Word> word, size_t reg)
{
    if (reg > m_ArgumentRegisters.size())
    {
        while (m_ArgumentRegisters.size() < reg - 1)
            m_ArgumentRegisters.push_back(nullptr);
        m_ArgumentRegisters.push_back(word);
    }
    else
    {
        m_ArgumentRegisters[reg - 1] = word;
    }
}

std::shared_ptr<Word> ArgumentRegisters::dereferenceRegister(size_t reg) const
{
    if (reg > m_ArgumentRegisters.size())
        return nullptr;
    return m_ArgumentRegisters[reg - 1]->dereference();
}

std::ostream &operator<<(std::ostream &os, const ArgumentRegisters &argReg)
{
    for (size_t i = 1; i <= argReg.m_ArgumentRegisters.size(); i++)
    {
        if (i != 1)
            os << std::endl;
        os << "A" << i << ": ";
        auto reg = argReg.dereferenceRegister(i);
        os << "\t";
        if (reg)
            reg->print(os);
        else
            os << "EMPTY";
    }
    return os;
}