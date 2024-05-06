#include "ConstNode.hpp"

ConstNode::ConstNode(const std::string &name)
    : TermNode(name)
{
}

void ConstNode::codegen(CompilationContext &cctx)
{
    if (cctx.mode() == CodeGenerationMode::HEAD)
    {
        cctx.addInstruction(std::make_shared<GetConstantInstruction>(m_Name, cctx.availableReg()));
    }
    else
    {
        cctx.addInstruction(std::make_shared<PutConstantInstruction>(m_Name, cctx.availableReg()));
    }

    cctx.setAvailableReg(cctx.availableReg() + 1);
}

TermNode::TermType ConstNode::type()
{
    return TermNode::CONST;
}

void ConstNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Start ConstNode]======" << std::endl;
    std::cout << indent << "Value: " << m_Name << std::endl;
    std::cout << indent << "=======[End ConstNode]======" << std::endl;
}