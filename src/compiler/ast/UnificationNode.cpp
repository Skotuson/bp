#include "UnificationNode.hpp"

UnificationNode::UnificationNode(TermNode *x, TermNode *y)
    : m_X(x),
      m_Y(y)
{
}

void UnificationNode::codegen(CompilationContext &cctx)
{
  m_X ->m_IsGoal = true;
  m_X ->m_IsArg = true;
  m_Y ->m_IsGoal = true;
  m_Y ->m_IsArg = true;
  
  m_X->codegen(cctx);
  m_Y->codegen(cctx);
  // TODO: non collision naming, add some legit name
  std::string tempVariable = "__temp" + std::to_string(cctx.allocate()) + "__";
  cctx.noteVariable(tempVariable);
  cctx.addInstruction(std::make_shared<GetVariableInstruction>(tempVariable, m_AvailableReg - 2, cctx.getVarOffset(tempVariable)));
  cctx.addInstruction(std::make_shared<GetVariableInstruction>(tempVariable, m_AvailableReg - 1, cctx.getVarOffset(tempVariable)));
}