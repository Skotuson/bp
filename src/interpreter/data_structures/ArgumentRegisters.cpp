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
    std::vector<Word *> cpy;
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

    std::vector<Word *> cpy;

    for (const auto &arg : argReg.m_ArgumentRegisters)
    {
        cpy.push_back(arg->clone());
    }

    // TODO: check for leaks

    m_ArgumentRegisters = cpy;
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
    Word *w = m_ArgumentRegisters[reg - 1];

    if (w->tag() == TAG::VARIABLE)
    {
        VariableWord *vw = static_cast<VariableWord *>(w);
        // TODO: handles only direct -> now
        if (vw->bound())
            return *vw->ref();
    }

    return m_ArgumentRegisters[reg - 1];
}

std::ostream &operator<<(std::ostream &os, const ArgumentRegisters &argReg)
{
    for (size_t i = 1; i <= argReg.m_ArgumentRegisters.size(); i++)
    {
        if (i != 1)
            os << std::endl;
        os << "A" << i << ": ";
        auto reg = argReg.m_ArgumentRegisters[i - 1];
        if (reg)
            reg->print(os);
        else
            os << "EMPTY";
    }
    return os;
}