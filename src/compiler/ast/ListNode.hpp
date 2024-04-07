#pragma once

#include "ComplexNode.hpp"

struct ListNode : public ComplexNode
{
    ListNode(const std::vector<TermNode *> &head,
             TermNode *tail = nullptr);
    ~ListNode(void) override;
    void codegen(CompilationContext &cctx) override;
    TermType type() override;
    void print(const std::string &indent = "") override;

    size_t arity(void) override;

    void unifyHead(CompilationContext &cctx);
    void unifyRHS(CompilationContext &cctx);
    void unifyArguments(CompilationContext &cctx, ProcessedComplex &processedComplex);

    std::vector<TermNode *> m_Head;
    TermNode *m_Tail = nullptr;
};