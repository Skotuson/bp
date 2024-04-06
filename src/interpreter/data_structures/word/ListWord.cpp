#include "ListWord.hpp"

ListWord::ListWord(void)
    : Word(TAG::LIST)
{
}

void ListWord::print(std::ostream &os) const
{
    os << "list";
}

std::shared_ptr<Word> ListWord::clone(void)
{
    return std::make_shared<ListWord>();
}

std::string ListWord::toString(void)
{
    return "";
}