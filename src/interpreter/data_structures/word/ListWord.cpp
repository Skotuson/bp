#include "ListWord.hpp"

ListWord::ListWord(size_t heapAddress)
    : Word(TAG::LIST),
      m_HeapAddress(heapAddress)
{
}

void ListWord::print(std::ostream &os) const
{
    os << "list";
}

std::shared_ptr<Word> ListWord::clone(void)
{
    return std::make_shared<ListWord>(m_HeapAddress);
}

std::string ListWord::toString(void)
{
    return "";
}