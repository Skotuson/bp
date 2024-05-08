#include "Fail.hpp"

std::shared_ptr<Instruction> Fail::clone(void)
{
    return std::make_shared<Fail>();
}

void Fail::execute(WAMState &state)
{
    std::shared_ptr<ChoicePoint> cp = state.stackAt(state.m_BacktrackRegister);
    if (cp)
    {
        // Reload argument registers
        state.m_ArgumentRegisters = cp->m_ArgumentRegisters;

        // Reset heap
        while (state.HReg() != cp->m_BH)
        {
            state.heapPop();
        }

        // Reset all variables instantiated since choice point was built
        while (state.TRReg() != cp->m_BTR)
        {
            std::shared_ptr<VariableWord> popped = state.trailTop();
            popped->unbind();
            *popped->ref() = popped;
            state.trailPop();
        }
        // Branch to next rule
        state.m_ProgramCounter = cp->m_FA;
    }
    // TODO: Experimental (check when choice point stack is empty)
    else if (state.m_BacktrackRegister == UNSET_REG)
        state.m_FailFlag = true;
}

void Fail::print(std::ostream &os) const
{
    os << "__fail__";
}