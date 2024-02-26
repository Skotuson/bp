#ifndef WORD_H
#define WORD_H

#include <string>

enum TAG
{
    CONSTANT,
    VARIABLE,
    LIST,
    STRUCTURE,
    S_POINTER, // Structure pointer
    REFERENCE
};

class Word
{
protected:
    Word(TAG tag);
    TAG m_Tag;
};

class ConstantWord : public Word
{
    ConstantWord(const std::string &value);
    std::string m_Value;
};

#endif // WORD_H