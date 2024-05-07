#pragma once

#include "Word.hpp"

#include <vector>

class ListWord : public Word
{
public:
    ListWord(size_t heapAddress, const std::vector<std::shared_ptr<Word>> &heapRef);
    void print(std::ostream &os) const override;
    std::shared_ptr<Word> clone(void) const override;
    std::string toString(void);

    size_t m_HeapAddress;
    const std::vector<std::shared_ptr<Word>> &m_HeapRef;
};