#include "Word.hpp"

Word::Word(TAG tag)
    : m_Tag(tag)
{
}

ConstantWord::ConstantWord(const std::string &value)
    : Word(TAG::CONSTANT),
      m_Value(value)
{
}

void ConstantWord::print(std::ostream &os)
{
    os << "constant -> " << m_Value;
}

Word *ConstantWord::clone(void)
{
    return new ConstantWord(m_Value);
}