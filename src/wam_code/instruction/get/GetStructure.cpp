#include "GetStructure.hpp"

GetStructure::GetStructure(const std::string &name, size_t argumentRegister, size_t arity)
    : m_Name(name),
      m_ArgumentRegister(argumentRegister),
      m_Arity(arity)
{
}

std::shared_ptr<Instruction> GetStructure::clone(void)
{
    return std::make_shared<GetStructure>(m_Name, m_ArgumentRegister, m_Arity);
}

void GetStructure::execute(WAMState &state)
{
    std::shared_ptr<Word> w = state.m_ArgumentRegisters.dereferenceRegister(m_ArgumentRegister);
    if (w->tag() == VARIABLE)
    {
        std::shared_ptr<VariableWord> vw = std::static_pointer_cast<VariableWord>(w);
        state.trailPush(vw);
        vw->bind(std::make_shared<StructurePointerWord>(state.HReg(), state.m_Heap));
        state.heapPush(std::make_shared<StructureWord>(m_Name, m_Arity, state.m_Heap, state.HReg()));
        state.setWriteMode();
    }

    else if (w->tag() == S_POINTER)
    {
        std::shared_ptr<StructurePointerWord> spw = std::static_pointer_cast<StructurePointerWord>(w);
        std::shared_ptr<StructureWord> sw = std::static_pointer_cast<StructureWord>(state.heapAt(spw->m_HeapAddress));
        if (sw->m_Functor == m_Name && sw->m_Arity == m_Arity)
        {
            state.setReadMode();
            state.m_StructurePointer = spw->m_HeapAddress + 1;
            return;
        }
        fail(state);
    }

    else
    {
        fail(state);
    }
}

void GetStructure::print(std::ostream &os) const
{
    os << "get-structure " << m_Name << "/" << m_Arity << " A" << m_ArgumentRegister;
}