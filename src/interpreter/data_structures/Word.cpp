#include "Word.hpp"

std::ostream &operator<<(std::ostream &os, const TAG &tag)
{
    switch (tag)
    {
    case TAG::CONSTANT:
        return os << "Constant";
    case TAG::LIST:
        return os << "List";
    case TAG::REFERENCE:
        return os << "Reference";
    case TAG::S_POINTER:
        return os << "Structure Pointer";
    case TAG::VARIABLE:
        return os << "Variable";
    case TAG::STRUCTURE:
        return os << "Structure";
    }
    return os;
}

Word::Word(TAG tag)
    : m_Tag(tag)
{
}

std::ostream &operator<<(std::ostream &os, const Word &word)
{
    word.print(os);
    return os;
}

ConstantWord::ConstantWord(const std::string &value)
    : Word(TAG::CONSTANT),
      m_Value(value)
{
}

void ConstantWord::print(std::ostream &os) const
{
    os << "constant -> " << m_Value;
}

Word *ConstantWord::clone(void)
{
    return new ConstantWord(m_Value);
}

TAG ConstantWord::tag(void)
{
    return m_Tag;
}

bool ConstantWord::compareToConst(ConstantWord *cword)
{
    return cword->m_Value == m_Value;
}

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

TAG VariableWord::tag(void)
{
    return m_Tag;
}

void VariableWord::setRef(Word **ref)
{
    m_Ref = ref;
}

Word **VariableWord::ref(void)
{
    return m_Ref;
}

bool VariableWord::bound(void)
{
    return m_Bound;
}

bool VariableWord::compareToConst(ConstantWord *cword)
{
    // TODO
    return false;
}
