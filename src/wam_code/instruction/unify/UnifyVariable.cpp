#include "UnifyVariable.hpp"

UnifyVariable::UnifyVariable(const std::string &name, size_t offset)
    : m_Name(name),
      m_Offset(offset)
{
}

std::shared_ptr<Instruction> UnifyVariable::clone(void)
{
    return std::make_shared<UnifyVariable>(m_Name, m_Offset);
}

void UnifyVariable::execute(WAMState &state)
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

void UnifyVariable::print(std::ostream &os) const
{
    os << "unifyv " << m_Name << "(" << m_Offset << ")";
}