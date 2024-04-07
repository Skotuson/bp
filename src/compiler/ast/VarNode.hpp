#pragma once

#include "TermNode.hpp"

struct VarNode : public TermNode
{
    VarNode(const std::string &name);
    void codegen(CompilationContext &cctx) override;
    TermType type() override;
    void print(const std::string &indent = "") override;
};
