#pragma once

#include "TermNode.hpp"

struct UnificationNode : public GoalNode
{
    UnificationNode(std::shared_ptr<TermNode> x, std::shared_ptr<TermNode> y);
    void codegen(CompilationContext &cctx) override;
    void print(const std::string &indent = "") override;
    std::shared_ptr<TermNode> m_X, m_Y;
};