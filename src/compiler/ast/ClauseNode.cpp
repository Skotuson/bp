#include "ClauseNode.hpp"
#include "../../wam_code/instruction/Instructions.hpp"

ClauseNode::ClauseNode(const std::string &head,
                       std::vector<std::shared_ptr<TermNode>> args,
                       std::vector<std::shared_ptr<GoalNode>> body)
    : m_Head(head + "/" + std::to_string(args.size())),
      m_Args(args),
      m_Body(body)
{
}

void ClauseNode::codegen(CompilationContext &cctx)
{
    std::shared_ptr<TableEntry> entry = cctx.get(m_Head);
    // Generate the initial mark instruction for first clause of the predicate name
    if (!entry->m_Generated)
    {
        cctx.addLabel(m_Head);
        cctx.addInstruction(std::make_shared<Mark>());
    }
    else
    {
        std::string label = m_Head + std::to_string(entry->m_Generated);
        cctx.addLabel(label);
    }

    ++entry->m_Generated;
    std::string retryLabel = entry->m_Generated == entry->m_Clauses ? "quit" : m_Head + std::to_string(entry->m_Generated);
    cctx.addInstruction(std::make_shared<RetryMeElse>(retryLabel));

    // Generate an allocate instruction and count the number of local variables needed during codegen.
    cctx.resetVariables();
    // Keep the pointer so the N can be changed after variables were counted.
    std::shared_ptr<Allocate> alloc = std::make_shared<Allocate>(0);
    cctx.addInstruction(alloc);
    
    cctx.setAvailableReg(1);
    cctx.resetAvailableArithmeticVariable();
    cctx.setHeadGenerationMode();
    for (size_t i = 0; i < m_Args.size(); i++)
    {
        // Load the arguments into argument registers
        m_Args[i]->codegen(cctx);
    }

    // All get instructions were carried out
    cctx.setBodyGenerationMode();
    for (size_t i = 0; i < m_Body.size(); i++)
    {
        cctx.setAvailableReg(1);
        m_Body[i]->codegen(cctx);
    }
    // Assign the N to generated allocate instruction
    alloc->m_N = cctx.allocate();
    cctx.addInstruction(std::make_shared<Return>());
}

void ClauseNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Start ClauseNode]======" << std::endl;
    std::cout << indent << "[m_Head] => " << m_Head << std::endl;
    std::cout << indent << "[m_Args] => " << std::endl;
    for (const auto &arg : m_Args)
        arg->print(indent + " ");
    std::cout << indent << "[m_Body] => " << std::endl;
    for (const auto &goal : m_Body)
        goal->print(indent + " ");
    std::cout << indent << "=======[End ClauseNode]======" << std::endl;
}