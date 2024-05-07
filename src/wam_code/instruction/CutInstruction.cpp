#include "CutInstruction.hpp"

std::shared_ptr<Instruction> CutInstruction::clone(void)
{
    return std::make_shared<CutInstruction>();
}

void CutInstruction::execute(WAMState &state)
{
    state.m_BacktrackRegister = state.stackAt(state.EReg())->m_BB;
}

void CutInstruction::print(std::ostream &os) const
{
    os << "cut (!)";
}