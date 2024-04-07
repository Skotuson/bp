#pragma once

#include "ClauseNode.hpp"

#include <memory>

struct ProgramNode : public Node
{
    void codegen(CompilationContext &cctx);
    void print(const std::string &indent = "") override;
    std::vector<std::shared_ptr<ClauseNode>> m_Clauses;
};