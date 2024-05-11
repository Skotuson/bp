#include "BinaryOperatorNode.hpp"

BinaryOperatorNode::BinaryOperatorNode(BinaryOperator op, std::shared_ptr<ExpressionNode> lhs, std::shared_ptr<ExpressionNode> rhs)
    : m_Operator(op),
      m_LHS(lhs),
      m_RHS(rhs)
{
}

void BinaryOperatorNode::codegen(CompilationContext &cctx)
{
}

void BinaryOperatorNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Start BinaryOperator]======" << std::endl;
    m_LHS->print(indent + " ");
    std::cout << m_Operator << std::endl;
    m_RHS->print(indent + " ");
    std::cout << indent << "=======[End BinaryOperator]======" << std::endl;
}