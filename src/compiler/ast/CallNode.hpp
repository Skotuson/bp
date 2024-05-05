#pragma once

#include "GoalNode.hpp"
#include "TermNode.hpp"

struct CallNode : public GoalNode
{
    CallNode(const std::string &name,
               std::vector<std::shared_ptr<TermNode>> args = std::vector<std::shared_ptr<TermNode>>());
    void codegen(CompilationContext &cctx) override;
    void print(const std::string &indent = "") override;

    std::string m_Name;
    std::vector<std::shared_ptr<TermNode>> m_Args;
};