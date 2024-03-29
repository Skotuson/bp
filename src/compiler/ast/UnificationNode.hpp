#pragma once

#include "TermNode.hpp"

struct UnificationNode : public GoalNode
{
    UnificationNode(TermNode *x, TermNode *y);
    std::string codegen(CompilationContext &cctx) override;
    TermNode *m_X, *m_Y;
};