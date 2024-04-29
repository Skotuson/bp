#pragma once

#include "TermNode.hpp"

struct UnificationNode : public GoalNode
{
    UnificationNode(TermNode *x, TermNode *y);
    void codegen(CompilationContext &cctx) override;
    void print(const std::string &indent = "") override;
    TermNode *m_X, *m_Y;
};