#include "CallNode.hpp"

#include "../../wam_code/instruction/Instructions.hpp"

CallNode::CallNode(const std::string &name, std::vector<std::shared_ptr<TermNode>> args)
    : m_Name(name),
      m_Args(args)
{
}

void CallNode::codegen(CompilationContext &cctx)
{
    for (const auto &arg : m_Args)
    {
        arg->codegen(cctx);
    }
    // Use the functor and arity to create a callName, e.g. s/2 for s(x,y)
    std::string callName = m_Name + "/" + std::to_string(m_Args.size());
    cctx.addInstruction(std::make_shared<Call>(callName));
    // Reset the avilable register back to 1
    cctx.setAvailableReg(1);
}

void CallNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Start CallNode]======" << std::endl;
    std::cout << indent << "[m_Name] => " << m_Name << "/" << m_Args.size() << std::endl;
    if (!m_Args.empty())
    {
        std::cout << indent << "[m_Args] => " << std::endl;
        for (const auto &arg : m_Args)
            arg->print(indent + " ");
    }
    std::cout << indent << "=======[End CallNode]======" << std::endl;
}
