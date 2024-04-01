#pragma once

#include "Word.hpp"

class ConstantWord : public Word
{
public:
    ConstantWord(const std::string &value);
    void print(std::ostream &os) const override;
    Word *clone(void) override;
    std::string toString(void);
    
    std::string value(void);

    bool compareToConst(ConstantWord *cword) override;

private:
    std::string m_Value;
};