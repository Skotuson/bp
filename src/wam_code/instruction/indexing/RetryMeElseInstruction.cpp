#include "RetryMeElseInstruction.hpp"

RetryMeElseInstruction::RetryMeElseInstruction(const std::string &label, size_t address)
    : BranchInstruction(label, address)
{
}

std::shared_ptr<Instruction> RetryMeElseInstruction::clone(void)
{
    return std::make_shared<RetryMeElseInstruction>(m_Label, m_Address);
}

void RetryMeElseInstruction::execute(WAMState &state)
{
    // Set next clause to the L (m_Address)
    std::shared_ptr<ChoicePoint> cp = state.stackAt(state.BReg());
    if (cp)
    {
        cp->m_FA = m_Address;
    }
}

void RetryMeElseInstruction::print(std::ostream &os) const
{
    os << "retry-me-else " << m_Label << "[" << m_Address << "]";
}