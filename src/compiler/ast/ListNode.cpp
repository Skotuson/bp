#include "ListNode.hpp"

ListNode::ListNode(const std::vector<TermNode *> &list, TermNode *tail)
    : ComplexNode(".")
{
    if (tail)
    {
        m_Head = list;
        m_Tail = tail;
    }
    else if (!list.empty())
    {
        {
            m_Head = {list.front()};
            m_Tail = new ListNode({list.begin() + 1, list.end()});
        }
    }
}

ListNode::~ListNode(void)
{
    for (TermNode *el : m_Head)
        delete el;
    delete m_Tail;
}

std::string ListNode::codegen(CompilationContext &cctx)
{
    std::string code = "";
    if (m_Head.empty())
    {
        if (!m_IsGoal)
        {
            cctx.addInstruction(std::make_shared<GetConstantInstruction>("[]", m_AvailableReg++));
        }
        else
        {
            cctx.addInstruction(std::make_shared<PutConstantInstruction>("[]", m_AvailableReg++));
        }
        return code;
    }

    if (m_IsGoal)
    {
                
        return code;
    }

    return code;
}

TermNode::TermType ListNode::type()
{
    return TermNode::LIST;
}

void ListNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Start ListNode]======" << std::endl;
    if (!m_Head.empty())
    {
        std::cout << indent << "[m_Head] => " << std::endl;
        for (const auto &term : m_Head)
            term->print(indent + " ");
    }
    if (m_Tail)
    {
        std::cout << indent << "[m_Tail] => " << std::endl;
        m_Tail->print(indent + " ");
    }
    std::cout << indent << "=======[End ListNode]======" << std::endl;
}

size_t ListNode::arity(void)
{
    return m_Head.size() + 1;
}

void ListNode::unifyHead(CompilationContext &cctx)
{
}

void ListNode::unifyRHS(CompilationContext &cctx)
{
}

void ListNode::unifyArguments(CompilationContext &cctx, ProcessedComplex &processedComplex)
{
}