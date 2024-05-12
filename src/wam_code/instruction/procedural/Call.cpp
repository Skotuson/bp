#include "Call.hpp"

Call::Call(const std::string &label, size_t address)
    : BranchInstruction(label, address)
{
}

std::shared_ptr<Instruction> Call::clone(void)
{
    return std::make_shared<Call>(m_Label, m_Address);
}

void Call::execute(WAMState &state)
{
    // Program counter already points to another instruction
    state.setCPReg(state.PC());
    // Branch to L (m_Address), with return address in CP.
    state.setPCReg(m_Address);
}

void Call::print(std::ostream &os) const
{
    os << "call " + m_Label << "[" << m_Address << "]";
}