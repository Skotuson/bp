#include "ListWord.hpp"

ListWord::ListWord(void)
    : Word(TAG::LIST)
{
}

void ListWord::print(std::ostream &os) const
{
    os << "list";
}

Word *ListWord::clone(void)
{
    return new ListWord();
}