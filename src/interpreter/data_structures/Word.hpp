#ifndef WORD_H
#define WORD_H

#include <string>
#include <iostream>

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
    virtual void print(std::ostream & os) = 0;
    virtual Word *clone(void) = 0;

protected:
    Word(TAG tag);
    TAG m_Tag;
};

class ConstantWord : public Word
{
public:
    ConstantWord(const std::string &value);
    void print(std::ostream & os) override;
    Word *clone(void) override;

private:
    std::string m_Value;
};

#endif // WORD_H