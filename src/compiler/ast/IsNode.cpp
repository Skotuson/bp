#include "IsNode.hpp"

IsNode::IsNode(std::shared_ptr<TermNode> x, std::shared_ptr<TermNode> y)
    : m_X(x),
      m_Y(y)
{
}

void IsNode::codegen(CompilationContext &cctx) override
{
}

void IsNode::print(const std::string &indent = "") override
{
    std::cout << indent << "=======[Start IsNode]======" << std::endl;
    m_X->print(indent + " ");
    m_Y->print(indent + " ");
    std::cout << indent << "=======[End IsNode]======" << std::endl;
}