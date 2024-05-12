#include "Mark.hpp"

std::shared_ptr<Instruction> Mark::clone(void)
{
    return std::make_shared<Mark>();
}

void Mark::execute(WAMState &state)
{
    // Build a new choice point up to the enviornment
    auto ncp = std::make_shared<ChoicePoint>(state.getArgumentRegisters(),
                                             state.EReg(),
                                             state.CPReg(),
                                             state.BReg(),
                                             state.TRReg(),
                                             state.HReg(),
                                             state.PCReg());
    state.stackPush(ncp);
    // Make it a current one (Set B register)
    state.setBReg(state.SReg() - 1);
}

void Mark::print(std::ostream &os) const
{
    os << "mark";
}