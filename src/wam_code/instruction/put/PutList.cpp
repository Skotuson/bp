#include "PutList.hpp"

#include "../../../word/ListWord.hpp"

PutList::PutList(size_t argumentRegister)
    : m_ArgumentRegister(argumentRegister)
{
}

std::shared_ptr<Instruction> PutList::clone(void)
{
    return std::make_shared<PutList>(m_ArgumentRegister);
}

void PutList::execute(WAMState &state)
{
    state.fillRegister(std::make_shared<ListWord>(state.HReg(), state.m_Heap), m_ArgumentRegister);
    state.setWriteMode();
}

void PutList::print(std::ostream &os) const
{
    os << "put-list A" << m_ArgumentRegister;
}