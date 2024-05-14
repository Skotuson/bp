#include "Cut.hpp"

std::shared_ptr<Instruction> Cut::clone(void)
{
    return std::make_shared<Cut>();
}

void Cut::execute(WAMState &state)
{
    state.setBReg(state.stackAt(state.EReg())->BBField());
}

void Cut::print(std::ostream &os) const
{
    os << "cut (!)";
}