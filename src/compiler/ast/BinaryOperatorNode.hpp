#pragma once

#include "ExpressionNode.hpp"

enum BinaryOperator
{
    PLUS,
    MINUS,
    MUL,
    DIV,
    LESS,
    LESSEQ,
    GREATER,
    GREATEREQ
};

struct BinaryOperatorNode : public ExpressionNode
{
public:
    BinaryOperatorNode(BinaryOperator op, std::shared_ptr<ExpressionNode> lhs, std::shared_ptr<ExpressionNode> rhs);
    void codegen(CompilationContext &cctx) override;
    void print(const std::string &indent = "") override;

private:
    BinaryOperator m_Operator;
    std::shared_ptr<ExpressionNode> m_LHS;
    std::shared_ptr<ExpressionNode> m_RHS;
};