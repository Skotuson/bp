#pragma once

#include "Word.hpp"

/**
 * @brief
 *
 */
class VariableWord : public Word
{
public:
    VariableWord(std::shared_ptr<Word> *ref, std::string name = "");
    void print(std::ostream &os) const override;
    std::shared_ptr<Word> clone(void) const override;
    std::string toString(void);
    TAG tag(void) override;

    std::shared_ptr<Word> dereference(void) const override;
    void bind(std::shared_ptr<Word> w);
    std::shared_ptr<Word> *ref(void) const;
    bool bound(void) const;

    void setVisited(bool visited);

private:
    std::shared_ptr<Word> *m_Ref;
    std::string m_Name;
    bool m_Visited = false;
};
