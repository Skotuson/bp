#include "PutListInstruction.hpp"

#include "../../../word/ListWord.hpp"

PutListInstruction::PutListInstruction(size_t argumentRegister)
    : m_ArgumentRegister(argumentRegister)
{
}

std::shared_ptr<Instruction> PutListInstruction::clone(void)
{
    return std::make_shared<PutListInstruction>(m_ArgumentRegister);
}

void PutListInstruction::execute(WAMState &state)
{
    state.fillRegister(std::make_shared<ListWord>(state.HReg(), state.m_Heap), m_ArgumentRegister);
    state.setWriteMode();
}

void PutListInstruction::print(std::ostream &os) const
{
    os << "put-list A" << m_ArgumentRegister;
}