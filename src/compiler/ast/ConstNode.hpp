#pragma once

#include "TermNode.hpp"

struct ConstNode : public TermNode
{
    ConstNode(const std::string &name);
    void codegen(CompilationContext &cctx) override;
    std::string codegen_arithmetic(CompilationContext &cctx) override;
    TermType type() override;
    void print(const std::string &indent = "") override;
};