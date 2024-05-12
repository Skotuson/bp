#include "GetVariable.hpp"

GetVariable::GetVariable(const std::string &name,
                                               size_t argumentRegister, size_t offset)
    : m_Name(name),
      m_ArgumentRegister(argumentRegister),
      m_Offset(offset)
{
}

std::shared_ptr<Instruction> GetVariable::clone(void)
{
    return std::make_shared<GetVariable>(m_Name, m_ArgumentRegister, m_Offset);
}

void GetVariable::execute(WAMState &state)
{
    std::shared_ptr<Word> X = state.dereferenceArgumentRegister(m_ArgumentRegister),
                          Y = state.stackAt(state.EReg())->m_Variables[m_Offset];
    clearPDL(state, X, Y);
}

void GetVariable::print(std::ostream &os) const
{
    os << "getv " << m_Name << "(" << m_Offset << ")"
       << " A" << m_ArgumentRegister;
}