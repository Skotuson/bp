#pragma once

#include "ComplexNode.hpp"

struct StructNode : public ComplexNode
{
    StructNode(const std::string &name,
               std::vector<TermNode *> args = std::vector<TermNode *>());
    ~StructNode(void) override;
    std::string codegen(CompilationContext &cctx) override;
    TermType type() override;
    void print(const std::string &indent = "") override;

    size_t arity(void) override;

    void unifyHead(CompilationContext &cctx) override;
    void unifyRHS(CompilationContext &cctx) override;
    void unifyArguments(CompilationContext &cctx, ProcessedComplex &processedComplex) override;
    
    std::vector<TermNode *> m_Args;
};