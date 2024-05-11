#pragma once

#include "GoalNode.hpp"
#include "TermNode.hpp"

struct IsNode : public GoalNode
{
    IsNode(std::shared_ptr<TermNode> x, std::shared_ptr<TermNode> y);
    void codegen(CompilationContext &cctx) override;
    void print(const std::string &indent = "") override;
    std::shared_ptr<TermNode> m_X, m_Y;
};