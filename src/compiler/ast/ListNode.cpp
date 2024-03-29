#include "ListNode.hpp"

ListNode::ListNode(const std::vector<TermNode *> &list, TermNode *tail)
    : TermNode(".")
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
    // TODO: decide how to represent empty list
    if (m_Head.empty())
        return "get empty-list";

    code += "get-list A" + std::to_string(m_AvailableReg++);

    return code;
}

TermNode::TermType ListNode::type()
{
    return TermNode::STRUCT;
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