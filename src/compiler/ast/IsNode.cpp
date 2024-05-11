#include "IsNode.hpp"

#include "UnificationNode.hpp"
#include "VarNode.hpp"

IsNode::IsNode(std::shared_ptr<TermNode> x, std::shared_ptr<TermNode> y)
    : m_X(x),
      m_Y(y)
{
}

void IsNode::codegen(CompilationContext &cctx)
{
    std::string varX, varY;
    varX = m_X->codegen_arithmetic(cctx);
    varY = m_Y->codegen_arithmetic(cctx);
    auto unif = std::make_shared<UnificationNode>(std::make_shared<VarNode>(varX, true), std::make_shared<VarNode>(varY, true));
    unif->codegen(cctx);
}

void IsNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Start IsNode]======" << std::endl;
    m_X->print(indent + " ");
    m_Y->print(indent + " ");
    std::cout << indent << "=======[End IsNode]======" << std::endl;
}