#include "PutStructure.hpp"

PutStructure::PutStructure(const std::string &name, size_t argumentRegister, size_t arity)
    : m_Name(name),
      m_ArgumentRegister(argumentRegister),
      m_Arity(arity)
{
}

std::shared_ptr<Instruction> PutStructure::clone(void)
{
    return std::make_shared<PutStructure>(m_Name, m_ArgumentRegister, m_Arity);
}

void PutStructure::execute(WAMState &state)
{
    state.fillRegister(std::make_shared<StructurePointerWord>(state.HReg(), state.m_Heap), m_ArgumentRegister);
    state.heapPush(std::make_shared<StructureWord>(m_Name, m_Arity, state.m_Heap, state.HReg()));
    state.setWriteMode();
}

void PutStructure::print(std::ostream &os) const
{
    os << "put-structure " << m_Name << "/" << m_Arity << " A" << m_ArgumentRegister;
}