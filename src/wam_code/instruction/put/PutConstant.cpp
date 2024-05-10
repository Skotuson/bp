#include "PutConstant.hpp"

PutConstant::PutConstant(const std::string &name, size_t argumentRegister)
    : m_Name(name),
      m_ArgumentRegister(argumentRegister)
{
}

std::shared_ptr<Instruction> PutConstant::clone(void)
{
    return std::make_shared<PutConstant>(m_Name, m_ArgumentRegister);
}

void PutConstant::execute(WAMState &state)
{
    state.fillRegister(std::make_shared<ConstantWord>(m_Name), m_ArgumentRegister);
}

void PutConstant::print(std::ostream &os) const
{
    os << "put-constant " << m_Name << " A" << m_ArgumentRegister;
}