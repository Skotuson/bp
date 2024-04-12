#pragma once

#include "GoalNode.hpp"

struct TermNode : public GoalNode
{
protected:
    TermNode(const std::string &name);
    std::string m_Name;

public:
    enum TermType
    {
        CONST,
        VAR,
        STRUCT,
        LIST
    };
    std::string name();
    virtual TermType type() = 0;
};
