#include "MarkInstruction.hpp"

std::shared_ptr<Instruction> MarkInstruction::clone(void)
{
    return std::make_shared<MarkInstruction>();
}

void MarkInstruction::execute(WAMState &state)
{
    // Build a new choice point up to the enviornment
    auto ncp = std::make_shared<ChoicePoint>(state.m_ArgumentRegisters,
                                             state.EReg(),
                                             state.m_ContinuationPointer,
                                             state.BReg(),
                                             state.TRReg(),
                                             state.HReg(),
                                             state.m_ProgramCounter);
    state.stackPush(ncp);
    // Set E and B registers
    // Make it a current one
    state.m_BacktrackRegister /*= state.m_EnvironmentRegister*/ = state.SReg() - 1;
}

void MarkInstruction::print(std::ostream &os) const
{
    os << "mark";
}