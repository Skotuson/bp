#include "ChoicePoint.hpp"

ChoicePoint::ChoicePoint(const ArgumentRegisters &argReg, size_t bce, size_t bcp,
                         size_t bb, size_t btr, size_t fa)
    : m_ArgumentRegisters(argReg),
      m_BCE(bce),
      m_BCP(bcp),
      m_BB(bb),
      m_BTR(btr),
      m_FA(fa)
{
}

ChoicePoint::~ChoicePoint(void)
{
  for (auto var : m_Variables)
  {
    delete var;
  }
}

std::ostream &operator<<(std::ostream &os, const ChoicePoint &cp)
{
  os << "-------------------------" << std::endl;
  os << "START-ARG-REGS" << std::endl;
  os << cp.m_ArgumentRegisters << std::endl;
  os << "END-ARG-REGS" << std::endl;
  os << "BCP:" << cp.m_BCP << std::endl;
  os << "BCE:" << cp.m_BCE << std::endl;
  os << "BB:" << cp.m_BB << std::endl;
  os << "BTR:" << cp.m_BTR << std::endl;
  os << "FA:" << cp.m_FA << std::endl;
  for (const auto &w : cp.m_Variables)
  {
    os << *w << std::endl;
  }
  os << "-------------------------";
  return os;
}