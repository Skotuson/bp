#pragma once

#include "Word.hpp"

class ListWord : public Word
{
public:
    ListWord(void);
    void print(std::ostream &os) const override;
    Word *clone(void) override;
    std::string toString(void);

private:
};