#pragma once

#include "Word.hpp"

class StructureWord : public Word
{
public:
    StructureWord(const std::string &functor, size_t arity);
    void print(std::ostream &os) const override;
    Word *clone(void) override;
    std::string toString(void);

    bool compareToStruct(StructureWord *sword) override;

    std::string m_Functor;
    size_t m_Arity;

private:
};