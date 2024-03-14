#ifndef WORD_H
#define WORD_H

#include <string>
#include <iostream>

enum TAG
{
    REFERENCE = 1,
    VARIABLE = 2,
    CONSTANT = 3,
    LIST = 4,
    S_POINTER = 5, // Structure pointer
    STRUCTURE = 6
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

    void setRef(Word **ref);
    Word **ref(void);
    bool bound(void);

    virtual bool compareToConst(ConstantWord *cword) override;

private:
    Word **m_Ref = nullptr;
    bool m_Bound = false;
};

#endif // WORD_H