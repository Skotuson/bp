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
            popped->bind(popped); 
            state.trailPop();
        }
        // Branch to next rule
        state.m_ProgramCounter = cp->m_FA;
    }
    // Backtracking to an empty stack -> fail.
    else if (state.m_BacktrackRegister == UNSET_REG)
        state.m_FailFlag = true;
}

void Fail::print(std::ostream &os) const
{
    os << "__fail__";
}