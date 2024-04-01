#pragma once

#include <string>
#include <iostream>

//#include "ConstantWord.hpp"
//#include "StructureWord.hpp"

enum TAG
{
    REFERENCE = 0,
    VARIABLE = 1,
    CONSTANT = 2,
    LIST = 3,
    S_POINTER = 4, // Structure pointer
    STRUCTURE = 5
};

std::ostream &operator<<(std::ostream &os, const TAG &tag);

class ConstantWord;
class StructureWord;

class Word
{
public:
    virtual ~Word() = default;
    virtual void print(std::ostream &os) const = 0;
    virtual Word *clone(void) = 0;
    virtual std::string toString(void) = 0;
    virtual TAG tag(void);

    virtual Word *dereference(void);

    virtual bool compareToConst(ConstantWord *cword);
    virtual bool compareToStruct(StructureWord *sword);

    friend std::ostream &operator<<(std::ostream &os, const Word &word);

protected:
    Word(TAG tag);
    TAG m_Tag;
};