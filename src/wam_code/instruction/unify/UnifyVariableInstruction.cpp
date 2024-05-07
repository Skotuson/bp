#include "UnifyVariableInstruction.hpp"

UnifyVariableInstruction::UnifyVariableInstruction(const std::string &name, size_t offset)
    : m_Name(name),
      m_Offset(offset)
{
}

std::shared_ptr<Instruction> UnifyVariableInstruction::clone(void)
{
    return std::make_shared<UnifyVariableInstruction>(m_Name, m_Offset);
}

void UnifyVariableInstruction::execute(WAMState &state)
{
    std::shared_ptr<Word> w = state.stackAt(state.EReg())->m_Variables[m_Offset]->dereference();
    if (!state.readMode())
    {
        state.heapPush(w);
        return;
    }

    std::shared_ptr<Word> sp = state.heapAt(state.SPReg())->dereference();
    // unify (similiar to getv)
    clearPDL(state, w, sp);
    state.m_StructurePointer++;
}

void UnifyVariableInstruction::print(std::ostream &os) const
{
    os << "unifyv " << m_Name << "(" << m_Offset << ")";
}