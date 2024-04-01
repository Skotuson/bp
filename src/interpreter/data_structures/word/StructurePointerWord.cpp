#include "StructurePointerWord.hpp"

StructurePointerWord::StructurePointerWord(size_t heapAddress)
    : Word(TAG::S_POINTER),
      m_HeapAddress(heapAddress)
{
}

void StructurePointerWord::print(std::ostream &os) const
{
    os << "structure-pointer -> HEAP-BOT + " << m_HeapAddress;
}

Word *StructurePointerWord::clone(void)
{
    return new StructurePointerWord(m_HeapAddress);
}