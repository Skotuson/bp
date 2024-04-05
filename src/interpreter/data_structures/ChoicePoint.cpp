#include "ChoicePoint.hpp"

#include <limits>

ChoicePoint::ChoicePoint(const ArgumentRegisters &argReg, size_t bce, size_t bcp,
                         size_t bb, size_t btr, size_t bh, size_t fa)
    : m_ArgumentRegisters(argReg),
      m_BCE(bce),
      m_BCP(bcp),
      m_BB(bb),
      m_BTR(btr),
      m_BH(bh),
      m_FA(fa)
{
}

std::ostream &operator<<(std::ostream &os, const ChoicePoint &cp)
{
  auto format = [](size_t n)
  {
    return n == std::numeric_limits<size_t>::max() ? "xxx" : std::to_string(n);
  };

  os << "-------------------------" << std::endl;
  os << "START-ARG-REGS" << std::endl;
  os << cp.m_ArgumentRegisters << std::endl;
  os << "END-ARG-REGS" << std::endl;
  os << "BCP:" << format(cp.m_BCP) << std::endl;
  os << "BCE:" << format(cp.m_BCE) << std::endl;
  os << "BB:" << format(cp.m_BB) << std::endl;
  os << "BTR:" << format(cp.m_BTR) << std::endl;
  os << "BH:" << format(cp.m_BH) << std::endl;
  os << "FA:" << format(cp.m_FA) << std::endl;
  for (size_t i = 0; i < cp.m_Variables.size(); i++)
  {
    os << "[" << &cp.m_Variables[i] << "]" << *cp.m_Variables[i] << std::endl;
  }
  os << "-------------------------";
  return os;
}