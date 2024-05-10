#include "Cut.hpp"

std::shared_ptr<Instruction> Cut::clone(void)
{
    return std::make_shared<Cut>();
}

void Cut::execute(WAMState &state)
{
    state.m_BacktrackRegister = state.stackAt(state.EReg())->m_BB;
}

void Cut::print(std::ostream &os) const
{
    os << "cut (!)";
}