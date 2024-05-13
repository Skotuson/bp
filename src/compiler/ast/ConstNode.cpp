#include "ConstNode.hpp"

#include "VarNode.hpp"
#include "UnificationNode.hpp"
#include "../../desugar/Desugar.hpp"
#include "../../wam_code/instruction/Instructions.hpp"

ConstNode::ConstNode(const std::string &name)
    : TermNode(name)
{
}

void ConstNode::codegen(CompilationContext &cctx)
{
    // Generating get instructions
    if (cctx.mode() == CodeGenerationMode::HEAD)
    {
        cctx.addInstruction(std::make_shared<GetConstant>(m_Name, cctx.availableReg()));
    }

    // Generating put instructions
    else
    {
        cctx.addInstruction(std::make_shared<PutConstant>(m_Name, cctx.availableReg()));
    }

    // TODO: see if can be moved elsewhere
    cctx.setAvailableReg(cctx.availableReg() + 1);
}

std::string ConstNode::codegen_arithmetic(CompilationContext &cctx)
{
    // Get next free arithmetic variable
    std::string varName = cctx.getAvailableArithmeticVariable();
    std::shared_ptr<TermNode> constant = std::make_shared<ConstNode>(m_Name);
    // Generate an unification instruction to bind the constant with the arithmetic variable
    auto unif = std::make_shared<UnificationNode>(std::make_shared<VarNode>(varName, true), constant);
    cctx.incrementAvailableArithmeticVariable();
    unif->codegen(cctx);
    // Return the node's arithmetic variable
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