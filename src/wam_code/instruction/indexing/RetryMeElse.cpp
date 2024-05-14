#include "RetryMeElse.hpp"

RetryMeElse::RetryMeElse(const std::string &label, size_t address)
    : BranchInstruction(label, address)
{
}

std::shared_ptr<Instruction> RetryMeElse::clone(void)
{
    return std::make_shared<RetryMeElse>(m_Label, m_Address);
}

void RetryMeElse::execute(WAMState &state)
{
    // Set next clause to the L (m_Address)
    std::shared_ptr<ChoicePoint> cp = state.stackAt(state.BReg());
    if (cp)
    {
        cp->setFAField(m_Address);
    }
}

void RetryMeElse::print(std::ostream &os) const
{
    os << "retry-me-else " << m_Label << "[" << m_Address << "]";
}