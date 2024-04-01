#include "StructureWord.hpp"

StructureWord::StructureWord(const std::string &functor, size_t arity)
    : Word(TAG::STRUCTURE),
      m_Functor(functor),
      m_Arity(arity)
{
}

void StructureWord::print(std::ostream &os) const
{
    os << "structure -> " << m_Functor << "/" << m_Arity;
}

Word *StructureWord::clone(void)
{
    return new StructureWord(m_Functor, m_Arity);
}

bool StructureWord::compareToStruct(StructureWord *sword)
{
    return m_Arity == sword->m_Arity && m_Functor == sword->m_Functor;
}