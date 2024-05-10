#include "Backtrack.hpp"

std::shared_ptr<Instruction> Backtrack::clone(void)
{
    return std::make_shared<Backtrack>();
}

void Backtrack::execute(WAMState &state)
{
    std::shared_ptr<ChoicePoint> cp = state.stackAt(state.m_BacktrackRegister);
    if (cp)
    {
        state.m_BacktrackRegister = cp->m_BB;
    }

    fail(state);

    // Discard the choice point (last clause in the chain failed)
    // Discard multiple choice points if cut is encountered
    while ((state.SReg() - 1) && state.SReg() - 1 != state.BReg())
    {
        state.stackPop();
    }
}

void Backtrack::print(std::ostream &os) const
{
    os << "backtrack";
}