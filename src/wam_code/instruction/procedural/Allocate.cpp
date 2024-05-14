#include "Allocate.hpp"

Allocate::Allocate(size_t n)
    : m_N(n)
{
}

std::shared_ptr<Instruction> Allocate::clone(void)
{
    return std::make_shared<Allocate>(m_N);
}

void Allocate::execute(WAMState &state)
{
    std::shared_ptr<ChoicePoint> cp = state.stackTop();
    // Allocate new environment to current choice point (Initialize all variables).
    cp->m_Variables.resize(m_N, nullptr);
    for (size_t i = 0; i < m_N; i++)
    {
        // In case the variable is a query variable and the environment is the first choice point, add the name.
        if (state.isQueryVariable(i) && state.SReg() == 1)
        {
            // Make the unbound variable word point to its memory location
            auto vw = std::make_shared<VariableWord>(&cp->m_Variables[i], state.getQueryVariableName(i));
            cp->m_Variables[i] = vw;
            state.addQueryWord(i, vw);
        }
        else
        {
            // In case its not a query variable, replace the name with a pattern __<number> to avoid name collisions
            cp->m_Variables[i] = std::make_shared<VariableWord>(&cp->m_Variables[i], "__" + std::to_string(state.getAllocatedVariables()));
            state.setAllocatedVariables(state.getAllocatedVariables() + 1);
        }
    }
    // Set E to this choice point
    state.setEReg(state.BReg());
}

void Allocate::print(std::ostream &os) const
{
    os << "allocate " << m_N;
}

void Allocate::setN(size_t n)
{
    m_N = n;
}