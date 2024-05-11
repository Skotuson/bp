#include "BinaryOperatorNode.hpp"

BinaryOperatorNode::BinaryOperatorNode(BinaryOperator op, std::shared_ptr<ExpressionNode> lhs, std::shared_ptr<ExpressionNode> rhs)
    : m_Operator(op),
      m_LHS(lhs),
      m_RHS(rhs)
{
}