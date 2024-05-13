#pragma once

#include "ComplexNode.hpp"

struct ListNode : public ComplexNode
{
    ListNode(const std::vector<std::shared_ptr<TermNode>> &head,
             std::shared_ptr<TermNode> tail = nullptr);
    void codegen(CompilationContext &cctx) override;
    std::string codegen_arithmetic(CompilationContext &cctx) override;
    TermType type() override;
    void print(const std::string &indent = "") override;

    size_t arity(void) override;

protected:
    void unifyHead(CompilationContext &cctx);
    void unifyRHS(CompilationContext &cctx);
    void unifyArguments(CompilationContext &cctx, ProcessedComplex &processedComplex);

private:
    std::vector<std::shared_ptr<TermNode>> m_Head;
    std::shared_ptr<TermNode> m_Tail = nullptr;
    // Whole list, used for RHS unification
    std::vector<std::shared_ptr<TermNode>> m_List;
};