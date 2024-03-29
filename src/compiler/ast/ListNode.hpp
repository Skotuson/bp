#pragma once

#include "TermNode.hpp"

struct ListNode : public TermNode
{
    ListNode(const std::vector<TermNode *> &head,
             TermNode *tail = nullptr);
    ~ListNode(void) override;
    std::string codegen(CompilationContext &cctx) override;
    TermType type() override;
    void print(const std::string &indent = "") override;

    std::vector<TermNode *> m_Head;
    TermNode *m_Tail = nullptr;
};