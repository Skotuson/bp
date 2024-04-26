#include "CutNode.hpp"

void CutNode::codegen(CompilationContext &cctx)
{
    cctx.addInstruction(std::make_shared<CutInstruction>());
}

void CutNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Cut Node]======" << std::endl;
}