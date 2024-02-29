#include "ChoicePoint.hpp"

ChoicePoint::ChoicePoint(const ArgumentRegisters &argReg, size_t bcp, size_t bce, size_t bb, size_t fa)
    : m_ArgumentRegisters(argReg),
      m_BCP(bcp),
      m_BCE(bce),
      m_BB(bb),
      m_FA(fa)
{
}

std::ostream &operator<<(std::ostream &os, const ChoicePoint &cp)
{
  os << "-------------------------" << std::endl;
  os << cp.m_ArgumentRegisters;
  os << "BCP:" << cp.m_BCP << std::endl;
  os << "BCE:" << cp.m_BCE << std::endl;
  os << "BB:" << cp.m_BB << std::endl;
  os << "FA:" << cp.m_FA << std::endl;
  os << "-------------------------";
  return os;
}