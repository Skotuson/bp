#pragma once

#include "TermNode.hpp"

struct ConstNode : public TermNode
{
    ConstNode(size_t value);
    void codegen(CompilationContext &cctx) override;
    TermType type() override;
    void print(const std::string &indent = "") override;
    int m_Value;
};