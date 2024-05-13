#pragma once

#include "ComplexNode.hpp"

struct StructNode : public ComplexNode
{
    StructNode(const std::string &name,
               std::vector<std::shared_ptr<TermNode>> args = std::vector<std::shared_ptr<TermNode>>());
    void codegen(CompilationContext &cctx) override;
    std::string codegen_arithmetic(CompilationContext &cctx) override;
    TermType type() override;
    void print(const std::string &indent = "") override;

    size_t arity(void) override;
    std::vector<std::shared_ptr<TermNode>> args(void);

protected:
    void unifyHead(CompilationContext &cctx) override;
    void unifyRHS(CompilationContext &cctx) override;
    void unifyArguments(CompilationContext &cctx, ProcessedComplex &processedComplex) override;

private:
    bool isBinaryOperator(void);
    std::vector<std::shared_ptr<TermNode>> m_Args;
};