#pragma once

#include "TermNode.hpp"

/**
 * @brief Represents a node that will be treated as an constant in the bytecode generation, either a lowercase atom or an empty list
 */
struct ConstNode : public TermNode
{
    ConstNode(const std::string &name);
    void codegen(CompilationContext &cctx) override;
    std::string codegen_arithmetic(CompilationContext &cctx) override;
    TermType type() override;
    void print(const std::string &indent = "") override;
};