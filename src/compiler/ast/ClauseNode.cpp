#include "ClauseNode.hpp"
#include "../../wam_code/instruction/Instructions.hpp"

ClauseNode::ClauseNode(const std::string &head,
                       std::vector<std::shared_ptr<TermNode>> args,
                       std::vector<std::shared_ptr<GoalNode>> body)
    // Encode the head as a functor and arity, e.g. s/2 for s(x,y)
    : m_Head(head + "/" + std::to_string(args.size())),
      m_Args(args),
      m_Body(body)
{
}

void ClauseNode::codegen(CompilationContext &cctx)
{
    // Get the symbol table entry for the head symbol
    std::shared_ptr<TableEntry> entry = cctx.get(m_Head);
    // Generate the initial mark instruction for first clause of the predicate name
    if (!entry->m_Generated)
    {
        // Note the label address
        cctx.addLabel(m_Head);
        cctx.addInstruction(std::make_shared<Mark>());
    }

    else
    {
        // Note the label for an alternative clause with the same predicate name
        std::string label = m_Head + std::to_string(entry->m_Generated);
        cctx.addLabel(label);
    }

    // Increment the number of generated clauses for this head symbol so far
    ++entry->m_Generated;
    // Generate retryLabel name for the next clause.
    // If the number of generated clauses equals that of total number of clauses, generate __quit label instead.
    std::string retryLabel = entry->m_Generated == entry->m_Clauses ? "__quit" : m_Head + std::to_string(entry->m_Generated);
    cctx.addInstruction(std::make_shared<RetryMeElse>(retryLabel));

    // Reset the variables for current clause
    cctx.resetVariables();
    // Generate an allocate instruction and count the number of local variables needed during codegen.
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
    alloc->setN(cctx.allocated());
    // Generate a return instruction
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

std::string ClauseNode::head(void)
{
    return m_Head;
}