#pragma once

#include "ClauseNode.hpp"

struct ProgramNode : public Node
{
    ~ProgramNode(void) override;
    virtual std::string codegen(CompilationContext &cctx);
    void print(const std::string &indent = "") override;
    std::vector<ClauseNode *> m_Clauses;
};