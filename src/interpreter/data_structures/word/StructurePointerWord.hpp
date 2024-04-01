#pragma once

#include "Word.hpp"

class StructurePointerWord : public Word
{
public:
    StructurePointerWord(size_t heapAddress);
    void print(std::ostream &os) const override;
    Word *clone(void) override;
    std::string toString(void);

    size_t m_HeapAddress;

private:
};