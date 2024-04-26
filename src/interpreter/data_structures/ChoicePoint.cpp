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
    return n == UNSET_REG ? "xxx" : std::to_string(n);
  };

  os << "-------------------------" << std::endl;
  os << cp.m_ArgumentRegisters << std::endl;
  os << "BCP:" << format(cp.m_BCP);
  os << "|BCE:" << format(cp.m_BCE);
  os << "|BB:" << format(cp.m_BB);
  os << "|BTR:" << format(cp.m_BTR);
  os << "|BH:" << format(cp.m_BH);
  os << "|FA:" << format(cp.m_FA) << std::endl;
  for (size_t i = 0; i < cp.m_Variables.size(); i++)
  {
    os << "\t" << "[" << &cp.m_Variables[i] << "]" << *cp.m_Variables[i] << std::endl;
  }
  os << "-------------------------";
  return os;
}