#include "ConstantWord.hpp"

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

std::string ConstantWord::toString(void)
{
    return m_Value;
}

std::string ConstantWord::value(void)
{
    return m_Value;
}

bool ConstantWord::compareToConst(ConstantWord *cword)
{
    return cword->m_Value == m_Value;
}