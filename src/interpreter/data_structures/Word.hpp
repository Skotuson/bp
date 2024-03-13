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

std::ostream &operator<<(std::ostream &os, const TAG &tag);

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

/**
 * @brief 
 *
 */
class VariableWord : public Word
{
public:
    VariableWord(Word **ref, bool bound = false);
    void print(std::ostream &os) const override;
    Word *clone(void) override;
    TAG tag(void) override;

    Word **ref(void);

    virtual bool compareToConst(ConstantWord *cword) override;

private:
    Word **m_Ref = nullptr;
    bool m_Bound = false;
};

class ReferenceWord : public Word
{
public:
    ReferenceWord(Word *word);
    void print(std::ostream &os) const override;
    Word *clone(void) override;
    TAG tag(void) override;

    Word *word(void);

    virtual bool compareToConst(ConstantWord *cword) override;

private:
    Word *m_Word;
};

#endif // WORD_H