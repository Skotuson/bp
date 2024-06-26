#include "Word.hpp"

std::ostream &operator<<(std::ostream &os, const TAG &tag)
{
    switch (tag)
    {
    case TAG::CONSTANT:
        return os << "Constant";
    case TAG::LIST:
        return os << "List";
    case TAG::REFERENCE:
        return os << "Reference";
    case TAG::S_POINTER:
        return os << "Structure Pointer";
    case TAG::VARIABLE:
        return os << "Variable";
    case TAG::STRUCTURE:
        return os << "Structure";
    }
    return os;
}

Word::Word(TAG tag)
    : m_Tag(tag)
{
}

TAG Word::tag(void)
{
    return m_Tag;
}

std::shared_ptr<Word> Word::dereference(void) const
{
    // As a shared_ptr is used, I can't return "this" pointer without enabling it.
    // The cloning however doesn't have any problems
    return clone();
}

bool Word::compareToConst(std::shared_ptr<ConstantWord> cword)
{
    return false;
}

bool Word::compareToStruct(std::shared_ptr<StructureWord> sword)
{
    return false;
}

std::ostream &operator<<(std::ostream &os, const Word &word)
{
    word.print(os);
    return os;
}