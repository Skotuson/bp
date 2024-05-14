#include "GetConstant.hpp"

GetConstant::GetConstant(const std::string &name, size_t argumentRegister)
    : m_Name(name),
      m_ArgumentRegister(argumentRegister)
{
}

std::shared_ptr<Instruction> GetConstant::clone(void)
{
    return std::make_shared<GetConstant>(m_Name, m_ArgumentRegister);
}

void GetConstant::execute(WAMState &state)
{
    std::shared_ptr<Word> reg = state.dereferenceArgumentRegister(m_ArgumentRegister);
    // Used to execute the compareToConst method
    std::shared_ptr<ConstantWord> cword = std::make_shared<ConstantWord>(m_Name);

    // Dereference result is a variable, trail
    if (reg && reg->tag() == TAG::VARIABLE)
    {
        std::shared_ptr<VariableWord> vw = std::static_pointer_cast<VariableWord>(reg);
        state.trailPush(vw); // Trail
        vw->bind(cword);
    }
    else if (!reg || !reg->compareToConst(cword))
    {
        fail(state);
    }
}

void GetConstant::print(std::ostream &os) const
{
    os << "get-constant " << m_Name << " A" << m_ArgumentRegister;
}