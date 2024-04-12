#include "ConstNode.hpp"

ConstNode::ConstNode(size_t value)
    : TermNode(std::to_string(value)),
      m_Value(value)
{
}

void ConstNode::codegen(CompilationContext &cctx)
{
    if (!m_IsGoal)
        cctx.addInstruction(std::make_shared<GetConstantInstruction>(m_Name, m_AvailableReg++));
    else
        cctx.addInstruction(std::make_shared<PutConstantInstruction>(m_Name, m_AvailableReg++));
}

TermNode::TermType ConstNode::type()
{
    return TermNode::CONST;
}

void ConstNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Start ConstNode]======" << std::endl;
    std::cout << indent << "Value: " << m_Value << std::endl;
    std::cout << indent << "=======[End ConstNode]======" << std::endl;
}