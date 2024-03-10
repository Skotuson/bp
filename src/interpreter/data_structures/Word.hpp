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
    virtual void print(std::ostream &os) const = 0;
    virtual Word *clone(void) = 0;
    virtual TAG tag(void) = 0;

    virtual bool compareToConst(ConstantWord *cword) = 0;

    friend std::ostream &operator<<(std::ostream &os, const Word &word);

protected:
    Word(TAG tag);
    TAG m_Tag;
};

class ConstantWord : public Word
{
public:
    ConstantWord(const std::string &value);
    void print(std::ostream &os) const override;
    Word *clone(void) override;
    TAG tag(void) override;

    bool compareToConst(ConstantWord *cword) override;

private:
    std::string m_Value;
};

class VariableWord : public Word
{
public:
    VariableWord(const std::string &name, size_t address);
    void print(std::ostream &os) const override;
    Word *clone(void) override;
    TAG tag(void) override;

    size_t address(void);

    virtual bool compareToConst(ConstantWord *cword) override;

private:
    std::string m_Name;
    size_t m_Address = 0;
};

class ReferenceWord : public Word
{
public:
    ReferenceWord(Word * word);
    void print(std::ostream &os) const override;
    Word *clone(void) override;
    TAG tag(void) override;

    virtual bool compareToConst(ConstantWord *cword) override;
private:
    Word * m_Word;
};

#endif // WORD_H