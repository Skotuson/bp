#include "TermNode.hpp"

TermNode::TermNode(const std::string &name)
    : m_Name(name)
{
}

std::string TermNode::name()
{
    return m_Name;
}