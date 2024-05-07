#include "PutVariable.hpp"

PutVariable::PutVariable(const std::string &name,
                         size_t argumentRegister, size_t offset)
    : m_Name(name),
      m_ArgumentRegister(argumentRegister),
      m_Offset(offset)
{
}

std::shared_ptr<Instruction> PutVariable::clone(void)
{
    return std::make_shared<PutVariable>(m_Name, m_ArgumentRegister, m_Offset);
}

void PutVariable::execute(WAMState &state)
{
    std::shared_ptr<ChoicePoint> cp = state.stackAt(state.EReg());
    std::shared_ptr<Word> word = cp->m_Variables[m_Offset]->dereference();
    if (word->tag() == TAG::VARIABLE)
    {
        std::shared_ptr<VariableWord> vw = std::make_shared<VariableWord>(&cp->m_Variables[m_Offset], "", true);
        state.fillRegister(vw, m_ArgumentRegister);
    }
    else
        state.fillRegister(word->clone(), m_ArgumentRegister);
}

void PutVariable::print(std::ostream &os) const
{
    os << "putv " << m_Name << "(" << m_Offset << ")"
       << " A" << m_ArgumentRegister;
}