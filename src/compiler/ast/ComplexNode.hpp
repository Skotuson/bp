#pragma once

#include "TermNode.hpp"

/**
 * @brief Represents either a list or a structure node
 */
struct ComplexNode;
using NestedPairing = std::map<ComplexNode *, size_t>;
using ProcessedComplex = std::map<Node *, std::string>;

struct ComplexNode : public TermNode
{
    ComplexNode(const std::string &name);

    virtual size_t arity(void) = 0;

    virtual void unifyHead(CompilationContext &cctx) = 0;
    virtual void unifyRHS(CompilationContext &cctx) = 0;
    virtual void unifyArguments(CompilationContext &cctx, ProcessedComplex &processedComplex) = 0;

    NestedPairing getNestedComplex(void);

    // Used when generating code for nested complex stuctures on the RHS of a goal
    NestedPairing m_Complex;
};