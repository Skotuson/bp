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

class ConstantWord;

class Word
{
public:
    virtual ~Word() = default;
    virtual void print(std::ostream &os) = 0;
    virtual Word *clone(void) = 0;
    virtual TAG tag(void) = 0;

    virtual bool compareToConst(ConstantWord *cword) = 0;

protected:
    Word(TAG tag);
    TAG m_Tag;
};

class ConstantWord : public Word
{
public:
    ConstantWord(const std::string &value);
    void print(std::ostream &os) override;
    Word *clone(void) override;
    TAG tag(void) override;

    bool compareToConst(ConstantWord * cword) override;

private:
    std::string m_Value;
};

#endif // WORD_H