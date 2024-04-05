#include "VarNode.hpp"

VarNode::VarNode(const std::string &name)
    : TermNode(name)
{
}

std::string VarNode::codegen(CompilationContext &cctx)
{
    cctx.noteVariable(m_Name);
    if (!m_IsGoal)
        cctx.addInstruction(
            std::make_shared<GetVariableInstruction>(m_Name, m_AvailableReg, cctx.getVarOffset(m_Name)));
    else
    {
        cctx.addVariable(m_Name);
        cctx.addInstruction(
            std::make_shared<PutVariableInstruction>(m_Name, m_AvailableReg, cctx.getVarOffset(m_Name)));
    }
    // TODO: beware of the ++ in the unused section when removing the string :)
    return (m_IsGoal ? "put " : "get ") + m_Name + " A" + std::to_string(m_AvailableReg++);
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