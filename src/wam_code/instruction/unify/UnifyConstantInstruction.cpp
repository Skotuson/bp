#include "UnifyConstantInstruction.hpp"

UnifyConstantInstruction::UnifyConstantInstruction(const std::string &name)
    : m_Name(name)
{
}

std::shared_ptr<Instruction> UnifyConstantInstruction::clone(void)
{
    return std::make_shared<UnifyConstantInstruction>(m_Name);
}

void UnifyConstantInstruction::execute(WAMState &state)
{
    if (!state.readMode())
    {
        state.heapPush(std::make_shared<ConstantWord>(m_Name));
        return;
    }

    std::shared_ptr<Word> w = state.heapAt(state.SPReg())->dereference();
    state.m_StructurePointer++;

    if (w->tag() == VARIABLE)
    {
        std::shared_ptr<VariableWord> vw = std::static_pointer_cast<VariableWord>(w);
        state.trailPush(vw);
        vw->bind(std::make_shared<ConstantWord>(m_Name));
    }

    else if (w->tag() == CONSTANT)
    {
        std::shared_ptr<ConstantWord> cw = std::static_pointer_cast<ConstantWord>(w);
        std::shared_ptr<ConstantWord> c = std::make_shared<ConstantWord>(m_Name);
        if (!cw->compareToConst(c))
        {
            fail(state);
        }
    }

    else
    {
        fail(state);
    }
}

void UnifyConstantInstruction::print(std::ostream &os) const
{
    os << "unify-constant " << m_Name;
}