#include "Return.hpp"

std::shared_ptr<Instruction> Return::clone(void)
{
    return std::make_shared<Return>();
}

void Return::execute(WAMState &state)
{
    std::shared_ptr<ChoicePoint> cp = state.stackAt(state.EReg());
    if (cp)
    {
        // Caller's return address
        state.setPCReg(cp->BCPField());
        // Caller's environment
        state.setEReg(cp->BCEField());
    }
    else
    {
        throw std::runtime_error("Invalid stack access");
    }
}

void Return::print(std::ostream &os) const
{
    os << "return";
}