#include "ClauseNode.hpp"

ClauseNode::ClauseNode(const std::string &head,
                       std::vector<TermNode *> args,
                       std::vector<GoalNode *> body)
    : m_Head(head + "/" + std::to_string(args.size())),
      // m_Head(head),
      m_Args(args),
      m_Body(body)
{
}

ClauseNode::~ClauseNode(void)
{
    for (TermNode *arg : m_Args)
        delete arg;
    for (GoalNode *goal : m_Body)
        delete goal;
}

std::string ClauseNode::codegen(CompilationContext &cctx)
{
    std::string code = "";
    TableEntry *entry = cctx.get(m_Head);
    // Generate the initial mark instruction for first clause of the predicate name
    if (!entry->m_Generated)
    {
        cctx.addLabel(m_Head);
        cctx.addInstruction(std::make_shared<MarkInstruction>());
        code += m_Head + ":\tmark\n";
    }
    else
    {
        std::string label = m_Head + std::to_string(entry->m_Generated);
        code += label + ":";
        cctx.addLabel(label);
    }

    ++entry->m_Generated;
    std::string retryLabel = entry->m_Generated == entry->m_Clauses ? "quit" : m_Head + std::to_string(entry->m_Generated);
    code += "\tretry-me-else " + retryLabel + "\n";
    cctx.addInstruction(std::make_shared<RetryMeElseInstruction>(retryLabel));

    // TODO: count all variables and complex objects (even nested) and generate the "n" afterwards
    cctx.resetVariables();
    std::shared_ptr<AllocateInstruction> alloc = std::make_shared<AllocateInstruction>(0);
    cctx.addInstruction(alloc);
    size_t allocInstrIdx = cctx.getCode().size() - 1;

    size_t currentArgumentRegister = 1;
    for (size_t i = 0; i < m_Args.size(); i++)
    {
        m_Args[i]->m_IsGoal = false;
        m_Args[i]->m_AvailableReg = currentArgumentRegister;
        // Load the arguments into argument registers
        code += "\t" + m_Args[i]->codegen(cctx) + "\n";
        currentArgumentRegister = m_Args[i]->m_AvailableReg;
    }

    // All get instructions were carried out
    currentArgumentRegister = 1;

    for (size_t i = 0; i < m_Body.size(); i++)
    {
        m_Body[i]->m_AvailableReg = currentArgumentRegister;
        code += "\t" + m_Body[i]->codegen(cctx) + "\n";
        currentArgumentRegister = m_Body[i]->m_AvailableReg;
    }

    // Generate allocate only if N is non-zero
    if (cctx.allocate())
        alloc->m_N = cctx.allocate();
    else
        cctx.getCode().deleteInstruction(allocInstrIdx);

    cctx.addInstruction(std::make_shared<ReturnInstruction>());
    return code + "\treturn\n";
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