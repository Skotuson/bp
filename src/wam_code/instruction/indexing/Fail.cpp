#include "Fail.hpp"

std::shared_ptr<Instruction> Fail::clone(void)
{
    return std::make_shared<Fail>();
}

void Fail::execute(WAMState &state)
{
    std::shared_ptr<ChoicePoint> cp = state.stackAt(state.BReg());
    if (cp)
    {
        // Reload argument registers
        state.setArgumentRegisters(cp->argumentRegisters());

        // Reset heap
        while (state.HReg() != cp->BHField())
        {
            state.heapPop();
        }

        // Reset all variables instantiated since choice point was built
        while (state.TRReg() != cp->BTRField())
        {
            std::shared_ptr<VariableWord> popped = state.trailTop();
            popped->bind(popped);
            state.trailPop();
        }
        // Branch to next rule
        state.setPCReg(cp->FAField());
    }
    // Backtracking to an empty stack -> fail.
    else if (state.BReg() == UNSET_REG)
    {
        state.setFailFlag(true);
    }
}

void Fail::print(std::ostream &os) const
{
    os << "__fail__";
}