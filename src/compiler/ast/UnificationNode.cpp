#include "UnificationNode.hpp"

UnificationNode::UnificationNode(TermNode *x, TermNode *y)
    : m_X(x),
      m_Y(y)
{
}

void UnificationNode::codegen(CompilationContext &cctx)
{
    
}