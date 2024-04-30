#include "VarNode.hpp"

VarNode::VarNode(const std::string &name)
    : TermNode(name)
{
}

void VarNode::codegen(CompilationContext &cctx)
{
    bool isWildcard = false;
    std::string name = m_Name;
    if (m_Name == "_")
    {
        name = cctx.generateTempVar();
    }
    cctx.noteVariable(name);
    if (!m_IsGoal)
        cctx.addInstruction(
            std::make_shared<GetVariableInstruction>(name, m_AvailableReg++, cctx.getVarOffset(name)));
    else
    {
        if (!isWildcard)
        {
            cctx.addVariable(name);
        }
        cctx.addInstruction(
            std::make_shared<PutVariableInstruction>(name, m_AvailableReg++, cctx.getVarOffset(name)));
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