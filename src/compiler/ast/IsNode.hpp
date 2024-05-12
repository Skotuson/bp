#pragma once

#include "GoalNode.hpp"
#include "TermNode.hpp"

struct IsNode : public GoalNode
{
    IsNode(std::shared_ptr<TermNode> lhs, std::shared_ptr<TermNode> rhs);
    void codegen(CompilationContext &cctx) override;
    void print(const std::string &indent = "") override;
    std::shared_ptr<TermNode> m_LHS, m_RHS;
};