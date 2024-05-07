#include "GetListInstruction.hpp"

#include "../../../word/ListWord.hpp"

GetListInstruction::GetListInstruction(size_t argumentRegister)
    : m_ArgumentRegister(argumentRegister)
{
}

std::shared_ptr<Instruction> GetListInstruction::clone(void)
{
    return std::make_shared<GetListInstruction>(m_ArgumentRegister);
}

void GetListInstruction::execute(WAMState &state)
{
    std::shared_ptr<Word> w = state.m_ArgumentRegisters.dereferenceRegister(m_ArgumentRegister);
    if (w->tag() == VARIABLE)
    {
        std::shared_ptr<VariableWord> vw = std::static_pointer_cast<VariableWord>(w);
        state.trailPush(vw);
        vw->bind(std::make_shared<ListWord>(state.HReg(), state.m_Heap));
        state.setWriteMode();
    }
    else if (w->tag() == LIST)
    {
        std::shared_ptr<ListWord> lw = std::static_pointer_cast<ListWord>(w);
        state.m_StructurePointer = lw->m_HeapAddress;
        state.setReadMode();
    }
    else
    {
        fail(state);
    }
}

void GetListInstruction::print(std::ostream &os) const
{
    os << "get-list A" << m_ArgumentRegister;
}