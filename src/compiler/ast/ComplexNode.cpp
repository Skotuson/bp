#include "ComplexNode.hpp"

ComplexNode::ComplexNode(const std::string &name)
    : TermNode(name)
{
}

NestedPairing ComplexNode::getNestedComplex(void)
{
    return m_Complex;
}