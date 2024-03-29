#include "UnificationNode.hpp"

UnificationNode::UnificationNode(TermNode *x, TermNode *y)
    : m_X(x),
      m_Y(y)
{
}

std::string UnificationNode::codegen(CompilationContext &cctx)
{
    return "";
}