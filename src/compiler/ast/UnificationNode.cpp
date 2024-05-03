#include "UnificationNode.hpp"
#include "StructNode.hpp"

UnificationNode::UnificationNode(std::shared_ptr<TermNode> x, std::shared_ptr<TermNode> y)
    : m_X(x),
      m_Y(y)
{
}

void UnificationNode::codegen(CompilationContext &cctx)
{
  std::vector<std::shared_ptr<TermNode>> args = {m_X, m_Y};
  std::shared_ptr<StructNode> unifStruct = std::make_shared<StructNode>("__id", args);
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