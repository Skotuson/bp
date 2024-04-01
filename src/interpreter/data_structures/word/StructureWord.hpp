#pragma once

#include "Word.hpp"

#include <vector>

class StructureWord : public Word
{
public:
    StructureWord(const std::string &functor, size_t arity, const std::vector<Word *> &heapRef, size_t offset);
    void print(std::ostream &os) const override;
    Word *clone(void) override;
    std::string toString(void);

    bool compareToStruct(StructureWord *sword) override;

    std::string m_Functor;
    size_t m_Arity;
    
    const std::vector<Word *> &m_HeapRef;
    size_t m_Offset;

private:
};