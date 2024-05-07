#include "PutConstantInstruction.hpp"

PutConstantInstruction::PutConstantInstruction(const std::string &name, size_t argumentRegister)
    : m_Name(name),
      m_ArgumentRegister(argumentRegister)
{
}

std::shared_ptr<Instruction> PutConstantInstruction::clone(void)
{
    return std::make_shared<PutConstantInstruction>(m_Name, m_ArgumentRegister);
}

void PutConstantInstruction::execute(WAMState &state)
{
    state.fillRegister(std::make_shared<ConstantWord>(m_Name), m_ArgumentRegister);
}

void PutConstantInstruction::print(std::ostream &os) const
{
    os << "put-constant " << m_Name << " A" << m_ArgumentRegister;
}