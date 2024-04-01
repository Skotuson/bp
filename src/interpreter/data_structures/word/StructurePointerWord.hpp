#pragma once

#include "Word.hpp"

#include <vector>

class StructurePointerWord : public Word
{
public:
    StructurePointerWord(size_t heapAddress, const std::vector<Word *> &heapRef);
    void print(std::ostream &os) const override;
    Word *clone(void) override;
    std::string toString(void);

    size_t m_HeapAddress;
    const std::vector<Word *> &m_HeapRef;

private:
};