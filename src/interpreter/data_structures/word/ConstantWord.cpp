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

std::shared_ptr<Word> ConstantWord::clone(void) const
{
    return std::make_shared<ConstantWord>(m_Value);
}

std::string ConstantWord::toString(void)
{
    return m_Value;
}

std::string ConstantWord::value(void)
{
    return m_Value;
}

bool ConstantWord::compareToConst(std::shared_ptr<ConstantWord> cword)
{
    return cword->m_Value == m_Value;
}