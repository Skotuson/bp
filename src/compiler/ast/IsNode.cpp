#include "IsNode.hpp"

#include "UnificationNode.hpp"
#include "VarNode.hpp"

IsNode::IsNode(std::shared_ptr<TermNode> x, std::shared_ptr<TermNode> y)
    : m_LHS(x),
      m_RHS(y)
{
}

void IsNode::codegen(CompilationContext &cctx)
{
    std::string varLHS, varRHS;
    // Generate the sequence of arithmetic operations for left-hand side
    varLHS = m_LHS->codegen_arithmetic(cctx);
    // Generate the sequence of arithmetic operations for right-hand side
    varRHS = m_RHS->codegen_arithmetic(cctx);

    // Check if both sides of the is operator are unifiable
    auto unif = std::make_shared<UnificationNode>(std::make_shared<VarNode>(varLHS, true), std::make_shared<VarNode>(varRHS, true));
    unif->codegen(cctx);
}

void IsNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Start IsNode]======" << std::endl;
    m_LHS->print(indent + " ");
    m_RHS->print(indent + " ");
    std::cout << indent << "=======[End IsNode]======" << std::endl;
}