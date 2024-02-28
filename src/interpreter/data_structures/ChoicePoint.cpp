#include "ChoicePoint.hpp"

ChoicePoint::ChoicePoint(const ArgumentRegisters &argReg, size_t bcp, size_t bce, size_t bb, size_t fa)
    : m_ArgumentRegisters(argReg),
      m_BCP(bcp),
      m_BCE(bce),
      m_BB(bb),
      m_FA(fa)
{
}