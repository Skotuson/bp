#include "VariableWord.hpp"

VariableWord::VariableWord(std::shared_ptr<Word> *ref, std::string name)
    : Word(TAG::VARIABLE),
      m_Ref(ref),
      m_Name(name)
{
}

void VariableWord::print(std::ostream &os) const
{
    if (bound())
    {
        os << "reference -> " << m_Ref << "[" << **m_Ref << "]";
    }
    else
    {
        os << "variable -> " << m_Ref;
    }
}

std::shared_ptr<Word> VariableWord::clone(void) const
{
    return std::make_shared<VariableWord>(m_Ref, m_Name);
}

std::string VariableWord::toString(void)
{
    if (dereference()->tag() != VARIABLE)
    {
        return dereference()->toString();
    }
    std::shared_ptr<VariableWord> vw = std::static_pointer_cast<VariableWord>(dereference());
    if (ref() != vw->ref())
    {
        return dereference()->toString();
    }
    return m_Name;
}

TAG VariableWord::tag(void)
{
    if (bound())
    {
        return TAG::REFERENCE;
    }

    return TAG::VARIABLE;
}

void VariableWord::bind(std::shared_ptr<Word> w)
{
    *ref() = w;
}

std::shared_ptr<Word> VariableWord::dereference(void) const
{
    return std::shared_ptr<Word>(*m_Ref);
}

std::shared_ptr<Word> *VariableWord::ref(void) const
{
    return m_Ref;
}

bool VariableWord::bound(void) const
{
    std::shared_ptr<VariableWord> vw = std::static_pointer_cast<VariableWord>(dereference());
    return ref() != vw->ref();
}