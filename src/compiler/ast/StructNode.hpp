#pragma once

#include "TermNode.hpp"

struct StructNode : public TermNode
{
    StructNode(const std::string &name,
               std::vector<TermNode *> args = std::vector<TermNode *>());
    ~StructNode(void) override;
    std::string codegen(CompilationContext &cctx) override;
    TermType type() override;
    void print(const std::string &indent = "") override;

    void unifyHead(CompilationContext &cctx);
    void unifyRHS(CompilationContext &cctx);
    bool hasNestedComplex(void);

    std::vector<TermNode *> m_Args;

    // Used when generating code for nested complex stuctures on the RHS of a goal
    // Use m_Complex size() to determine whether has any complex object
    std::map<StructNode *, size_t> m_Complex;
};