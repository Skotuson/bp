#include "CallNode.hpp"

#include "../../wam_code/instruction/CallInstruction.hpp"

CallNode::CallNode(const std::string &name, std::vector<std::shared_ptr<TermNode>> args)
    : m_Name(name),
      m_Args(args)
{
}

void CallNode::codegen(CompilationContext &cctx)
{
    for (const auto &arg : m_Args)
    {
        arg->m_IsGoal = true;
        arg->codegen(cctx);
    }
    std::string callName = m_Name + "/" + std::to_string(m_Args.size());
    cctx.addInstruction(std::make_shared<CallInstruction>(callName));
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
