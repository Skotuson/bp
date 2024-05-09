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
        if (state.m_QueryVariables.count(i) && state.SReg() == 1)
        {
            cp->m_Variables[i] = std::make_shared<VariableWord>(&cp->m_Variables[i], state.m_QueryVariables[i]);
        }
        else
        {
            cp->m_Variables[i] = std::make_shared<VariableWord>(&cp->m_Variables[i], "__" + std::to_string(state.m_AllocatedVariables++));
        }
    }
    // Set E to this choice point
    state.m_EnvironmentRegister = state.BReg();
}

void Allocate::print(std::ostream &os) const
{
    os << "allocate " << m_N;
}