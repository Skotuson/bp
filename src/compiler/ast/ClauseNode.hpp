#pragma once

#include "TermNode.hpp"

struct ClauseNode : public Node
{
    ClauseNode(const std::string &head,
               std::vector<TermNode *> args,
               std::vector<GoalNode *> body);

    ~ClauseNode(void) override;

    std::string codegen(CompilationContext &cctx) override;
    void print(const std::string &indent = "") override;
    std::string m_Head;
    std::vector<TermNode *> m_Args;
    std::vector<GoalNode *> m_Body;
};