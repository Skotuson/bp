#include "ComplexNode.hpp"

ComplexNode::ComplexNode(const std::string &name)
    : TermNode(name)
{
}

bool ComplexNode::hasNestedComplex(void)
{
    return m_Complex.size();
}

NestedPairing ComplexNode::getNestedComplex(void)
{
    return m_Complex;
}