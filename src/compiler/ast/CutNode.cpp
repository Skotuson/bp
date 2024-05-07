#include "CutNode.hpp"

#include "../../wam_code/instruction/Instructions.hpp"

void CutNode::codegen(CompilationContext &cctx)
{
    cctx.addInstruction(std::make_shared<Cut>());
}

void CutNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Cut Node]======" << std::endl;
}