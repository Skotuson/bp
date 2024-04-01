#include "StructureWord.hpp"

StructureWord::StructureWord(const std::string &functor, size_t arity, const std::vector<Word *> &heapRef, size_t offset)
    : Word(TAG::STRUCTURE),
      m_Functor(functor),
      m_Arity(arity),
      m_HeapRef(heapRef),
      m_Offset(offset)
{
}

void StructureWord::print(std::ostream &os) const
{
    os << "structure -> " << m_Functor << "/" << m_Arity;
}

Word *StructureWord::clone(void)
{
    return new StructureWord(m_Functor, m_Arity, m_HeapRef, m_Offset);
}

bool StructureWord::compareToStruct(StructureWord *sword)
{
    return m_Arity == sword->m_Arity && m_Functor == sword->m_Functor;
}

std::string StructureWord::toString(void)
{
    std::string str = m_Functor + "(";
    for(size_t i = 0; i < m_Arity; i++)
    {
        str += m_HeapRef[m_Offset + i + 1]->toString();
    }
    str += ")";
    return str;
}