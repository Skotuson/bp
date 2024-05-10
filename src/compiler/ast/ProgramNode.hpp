#pragma once

#include "ClauseNode.hpp"

#include <memory>

/**
 * @brief AST Node representing the structure of the source code (set of clauses).
 */
struct ProgramNode : public Node
{
    void codegen(CompilationContext &cctx);
    void print(const std::string &indent = "") override;

    // Set of clauses
    std::vector<std::shared_ptr<ClauseNode>> m_Clauses;
};