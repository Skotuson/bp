#include "StructurePointerWord.hpp"

StructurePointerWord::StructurePointerWord(size_t heapAddress, const std::vector<std::shared_ptr<Word>> &heapRef)
    : Word(TAG::S_POINTER),
      m_HeapAddress(heapAddress),
      m_HeapRef(heapRef)
{
}

void StructurePointerWord::print(std::ostream &os) const
{
    os << "structure-pointer -> H" << m_HeapAddress;
}

std::shared_ptr<Word> StructurePointerWord::clone(void) const
{
    return std::make_shared<StructurePointerWord>(m_HeapAddress, m_HeapRef);
}

std::string StructurePointerWord::toString(void)
{
    return m_HeapRef[m_HeapAddress]->toString();
}