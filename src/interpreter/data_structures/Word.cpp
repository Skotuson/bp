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

TAG ConstantWord::tag(void)
{
    return m_Tag;
}

bool ConstantWord::compareToConst(ConstantWord *cword)
{
    return cword->m_Value == m_Value;
}

VariableWord::VariableWord(const std::string &name)
    : Word(TAG::VARIABLE),
      m_Name(name)
{
}

void VariableWord::print(std::ostream &os)
{
    os << "variable " << m_Name << " -> " << m_Address;
}

Word *VariableWord::clone(void)
{
    return new VariableWord(m_Name);
}

TAG VariableWord::tag(void)
{
    return m_Tag;
}

bool VariableWord::compareToConst(ConstantWord *cword)
{
    // TODO
    return false;
}