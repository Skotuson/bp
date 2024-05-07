#include "ReturnInstruction.hpp"

std::shared_ptr<Instruction> ReturnInstruction::clone(void)
{
    return std::make_shared<ReturnInstruction>();
}

void ReturnInstruction::execute(WAMState &state)
{
    std::shared_ptr<ChoicePoint> cp = state.stackAt(state.EReg());
    if (cp)
    {
        // Caller's return address
        state.m_ProgramCounter = cp->m_BCP;
        // Caller's environment
        state.m_EnvironmentRegister = cp->m_BCE;
    }
    else
    {
        // TODO: mainly debug
        throw std::runtime_error("Invalid stack access");
    }
}

void ReturnInstruction::print(std::ostream &os) const
{
    os << "return";
}