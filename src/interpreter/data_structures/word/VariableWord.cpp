#include "VariableWord.hpp"

VariableWord::VariableWord(Word **ref, bool bound)
    : Word(TAG::VARIABLE),
      m_Ref(ref),
      m_Bound(bound)
{
}

void VariableWord::print(std::ostream &os) const
{
    if (m_Bound)
    {
        os << "reference -> " << m_Ref << "[" << **m_Ref << "]";
    }
    else
    {
        os << "variable -> " << m_Ref;
    }
}

Word *VariableWord::clone(void)
{
    return new VariableWord(m_Ref, m_Bound);
}

std::string VariableWord::toString(void)
{
    return dereference()->toString();
}

TAG VariableWord::tag(void)
{
    if (m_Bound)
    {
        return TAG::REFERENCE;
    }

    return TAG::VARIABLE;
}

void VariableWord::bind(Word *w)
{
    *ref() = w;
    m_Bound = true;
}

void VariableWord::unbind(void)
{
    m_Bound = false;
}

void VariableWord::setRef(Word **ref)
{
    m_Ref = ref;
}

Word *VariableWord::dereference(void)
{
    return (*m_Ref);
    // if (m_Bound)
    //{
    //     return (*m_Ref)->dereference();
    // }
    //
    // else
    //{
    //    return this;
    //}
}

Word **VariableWord::ref(void)
{
    return m_Ref;
}

bool VariableWord::bound(void)
{
    return m_Bound;
}