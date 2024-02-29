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
public:
    virtual Word *clone(void) = 0;

protected:
    Word(TAG tag);
    TAG m_Tag;
};

class ConstantWord : public Word
{
public:
    ConstantWord(const std::string &value);
    Word *clone(void) override;

private:
    std::string m_Value;
};

#endif // WORD_H