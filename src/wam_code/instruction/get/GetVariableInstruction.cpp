#include "GetVariableInstruction.hpp"

GetVariableInstruction::GetVariableInstruction(const std::string &name,
                                               size_t argumentRegister, size_t offset)
    : m_Name(name),
      m_ArgumentRegister(argumentRegister),
      m_Offset(offset)
{
}

std::shared_ptr<Instruction> GetVariableInstruction::clone(void)
{
    return std::make_shared<GetVariableInstruction>(m_Name, m_ArgumentRegister, m_Offset);
}

void GetVariableInstruction::execute(WAMState &state)
{
    std::shared_ptr<Word> X = state.m_ArgumentRegisters.dereferenceRegister(m_ArgumentRegister),
                          Y = state.stackAt(state.EReg())->m_Variables[m_Offset];
    clearPDL(state, X, Y);
}

void GetVariableInstruction::print(std::ostream &os) const
{
    os << "getv " << m_Name << "(" << m_Offset << ")"
       << " A" << m_ArgumentRegister;
}