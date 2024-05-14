#include "ListWord.hpp"

ListWord::ListWord(size_t heapAddress, const std::vector<std::shared_ptr<Word>> &heapRef)
    : Word(TAG::LIST),
      m_HeapAddress(heapAddress),
      m_HeapRef(heapRef)
{
}

void ListWord::print(std::ostream &os) const
{
    os << "list -> " << m_HeapAddress;
}

std::shared_ptr<Word> ListWord::clone(void) const
{
    return std::make_shared<ListWord>(m_HeapAddress, m_HeapRef);
}

std::string ListWord::toString(void)
{
    // Print the head | tail, where tail is printed recursively
    std::string str = "[" + m_HeapRef[m_HeapAddress]->toString() + "|";
    str += m_HeapRef[m_HeapAddress + 1]->toString() + "]";
    return str;
}