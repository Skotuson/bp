#pragma once

#include "Word.hpp"

/**
 * @brief
 *
 */
class VariableWord : public Word
{
public:
    VariableWord(std::shared_ptr<Word> *ref, std::string name = "", bool bound = false);
    void print(std::ostream &os) const override;
    std::shared_ptr<Word> clone(void) override;
    std::string toString(void);
    TAG tag(void) override;

    std::shared_ptr<Word> dereference(void) override;

    void bind(void);
    void bind(std::shared_ptr<Word> w);
    void unbind(void);
    std::shared_ptr<Word> *ref(void);
    bool bound(void) const;

private:
    std::shared_ptr<Word> *m_Ref;
    std::string m_Name;
    bool m_Bound;
};
