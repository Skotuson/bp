#include "VariableWord.hpp"

VariableWord::VariableWord(std::shared_ptr<Word> *ref, std::string name, bool bound)
    : Word(TAG::VARIABLE),
      m_Ref(ref),
      m_Name(name),
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

std::shared_ptr<Word> VariableWord::clone(void)
{
    return std::make_shared<VariableWord>(m_Ref, m_Name, m_Bound);
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
    if (m_Bound)
    {
        return TAG::REFERENCE;
    }

    return TAG::VARIABLE;
}

void VariableWord::bind(void)
{
    m_Bound = true;
}

void VariableWord::bind(std::shared_ptr<Word> w)
{
    *ref() = w;
    m_Bound = true;
}

void VariableWord::unbind(void)
{
    m_Bound = false;
}

void VariableWord::setRef(std::shared_ptr<Word> *ref)
{
    m_Ref = ref;
}

std::shared_ptr<Word> VariableWord::dereference(void)
{
    return std::shared_ptr<Word>(*m_Ref);
}

std::shared_ptr<Word> *VariableWord::ref(void)
{
    return m_Ref;
}

bool VariableWord::bound(void)
{
    return m_Bound;
}