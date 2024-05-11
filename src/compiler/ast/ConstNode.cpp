#include "ConstNode.hpp"

#include "VarNode.hpp"
#include "UnificationNode.hpp"
#include "../../wam_code/instruction/Instructions.hpp"

ConstNode::ConstNode(const std::string &name)
    : TermNode(name)
{
}

void ConstNode::codegen(CompilationContext &cctx)
{
    if (cctx.mode() == CodeGenerationMode::HEAD)
    {
        cctx.addInstruction(std::make_shared<GetConstant>(m_Name, cctx.availableReg()));
    }
    else
    {
        cctx.addInstruction(std::make_shared<PutConstant>(m_Name, cctx.availableReg()));
    }

    cctx.setAvailableReg(cctx.availableReg() + 1);
}

std::string ConstNode::codegen_arithmetic(CompilationContext &cctx)
{
    std::string varName = cctx.getAvailableArithmeticVariable();
    auto unif = std::make_shared<UnificationNode>(std::make_shared<VarNode>(varName, true), std::make_shared<ConstNode>(m_Name));
    cctx.incrementAvailableArithmeticVariable();
    unif->codegen(cctx);
    return varName;
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