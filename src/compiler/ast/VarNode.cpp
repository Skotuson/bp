#include "VarNode.hpp"

VarNode::VarNode(const std::string &name)
    : TermNode(name)
{
}

void VarNode::codegen(CompilationContext &cctx)
{
    cctx.noteVariable(m_Name);
    if (!m_IsGoal)
        cctx.addInstruction(
            std::make_shared<GetVariableInstruction>(m_Name, m_AvailableReg++, cctx.getVarOffset(m_Name)));
    else
    {
        cctx.addVariable(m_Name);
        cctx.addInstruction(
            std::make_shared<PutVariableInstruction>(m_Name, m_AvailableReg++, cctx.getVarOffset(m_Name)));
    }
}

TermNode::TermType VarNode::type()
{
    return TermNode::VAR;
}

void VarNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Start VarNode]======" << std::endl;
    std::cout << indent << "[m_Name] => " << m_Name << std::endl;
    std::cout << indent << "=======[End VarNode]======" << std::endl;
}