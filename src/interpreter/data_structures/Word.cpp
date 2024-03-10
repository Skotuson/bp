#include "Word.hpp"

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

VariableWord::VariableWord(const std::string &name, size_t address)
    : Word(TAG::VARIABLE),
      m_Name(name),
      m_Address(address)
{
}

void VariableWord::print(std::ostream &os) const
{
    os << "variable " << m_Name << " -> " << m_Address;
}

Word *VariableWord::clone(void)
{
    return new VariableWord(m_Name, m_Address);
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

ReferenceWord::ReferenceWord(Word *word)
    : Word(TAG::REFERENCE),
      m_Word(word)
{
}

void ReferenceWord::print(std::ostream &os) const
{
    os << "reference "
       << " -> "
       << "[" << *m_Word << "]";
}

Word *ReferenceWord::clone(void)
{
    return new ReferenceWord(m_Word);
}

TAG ReferenceWord::tag(void)
{
    return TAG::REFERENCE;
}

bool ReferenceWord::compareToConst(ConstantWord *cword)
{
    return false;
}