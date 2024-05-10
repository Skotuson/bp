#include "VarNode.hpp"
#include "../../wam_code/instruction/Instructions.hpp"

VarNode::VarNode(const std::string &name, bool isWildcard)
    : TermNode(name),
      m_IsWildcard(isWildcard)
{
}

void VarNode::codegen(CompilationContext &cctx)
{
    cctx.noteVariable(m_Name);
    if (cctx.mode() == CodeGenerationMode::HEAD)
    {
        cctx.addInstruction(
            std::make_shared<GetVariable>(m_Name, cctx.availableReg(), cctx.getVarOffset(m_Name)));
    }
    else
    {
        if (!m_IsWildcard)
        {
            cctx.addVariable(m_Name);
        }
        cctx.addInstruction(
            std::make_shared<PutVariable>(m_Name, cctx.availableReg(), cctx.getVarOffset(m_Name)));
    }

    cctx.setAvailableReg(cctx.availableReg() + 1);
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