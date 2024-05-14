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

const ArgumentRegisters &ChoicePoint::argumentRegisters(void)
{
  return m_ArgumentRegisters;
}

size_t ChoicePoint::BCEField(void)
{
  return m_BCE;
}

size_t ChoicePoint::BCPField(void)
{
  return m_BCP;
}

size_t ChoicePoint::BBField(void)
{
  return m_BB;
}

size_t ChoicePoint::BTRField(void)
{
  return m_BTR;
}

size_t ChoicePoint::BHField(void)
{
  return m_BH;
}

size_t ChoicePoint::FAField(void)
{
  return m_FA;
}

void ChoicePoint::setBCEField(size_t bce)
{
  m_BCE = bce;
}

void ChoicePoint::setBCPField(size_t bcp)
{
  m_BCP = bcp;
}

void ChoicePoint::setBBField(size_t bb)
{
  m_BB = bb;
}

void ChoicePoint::setBTRField(size_t btr)
{
  m_BTR = btr;
}

void ChoicePoint::setBHField(size_t bh)
{
  m_BH = bh;
}

void ChoicePoint::setFAField(size_t fa)
{
  m_FA = fa;
}

std::vector<std::shared_ptr<Word>> &ChoicePoint::localVariables(void)
{
  return m_Variables;
}

std::ostream &operator<<(std::ostream &os, const ChoicePoint &cp)
{
  auto format = [](size_t n)
  {
    return n == std::numeric_limits<size_t>::max() - 1 ? "xxx" : std::to_string(n);
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
    os << "\t offset " << i
       << " [" << &cp.m_Variables[i] << "]" << *cp.m_Variables[i] << std::endl;
  }
  os << "-------------------------";
  return os;
}