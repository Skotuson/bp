#pragma once

#include "Word.hpp"

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
    std::string toString(void);
    TAG tag(void) override;

    Word *dereference(void) override;

    void bind(Word * w);
    void unbind(void);
    void setRef(Word **ref);
    Word **ref(void);
    bool bound(void);

private:
    Word **m_Ref = nullptr;
    bool m_Bound = false;
};
