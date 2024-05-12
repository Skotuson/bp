#pragma once

#include "GoalNode.hpp"
#include "TermNode.hpp"

/**
 * @brief Represents a node that will get translated to a call instruction in the bytecode.
 * In the source code, this is a predicate in the body of a rule.
 */
struct CallNode : public GoalNode
{
    CallNode(const std::string &name,
             std::vector<std::shared_ptr<TermNode>> args = std::vector<std::shared_ptr<TermNode>>());
    void codegen(CompilationContext &cctx) override;
    void print(const std::string &indent = "") override;

private:
    std::string m_Name;
    std::vector<std::shared_ptr<TermNode>> m_Args;
};