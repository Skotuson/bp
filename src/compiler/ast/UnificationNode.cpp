#include "UnificationNode.hpp"
#include "StructNode.hpp"

#include "../../preprocessor/Preprocessor.hpp"

UnificationNode::UnificationNode(TermNode *x, TermNode *y)
    : m_X(x),
      m_Y(y)
{
}

void UnificationNode::codegen(CompilationContext &cctx)
{
  StructNode* unifStruct = new StructNode("__id", {m_X, m_Y});
  unifStruct->m_AvailableReg = m_AvailableReg;
  unifStruct->codegen(cctx);
}

void UnificationNode::print(const std::string &indent)
{
  std::cout << indent << "=======[Start UnificationNode]======" << std::endl;
  m_X->print(indent + "");
  m_Y->print(indent + "");
  std::cout << indent << "=======[End UnificationNode]======" << std::endl;
}