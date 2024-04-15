#include "UnificationNode.hpp"

UnificationNode::UnificationNode(TermNode *x, TermNode *y)
    : m_X(x),
      m_Y(y)
{
}

void UnificationNode::codegen(CompilationContext &cctx)
{
  std::vector<TermNode*> args = {m_X, m_Y};

  for(const auto &arg : args)
  {
    arg->m_IsArg = true;
    arg->m_IsGoal = true;
    arg->m_AvailableReg = m_AvailableReg;
    arg->codegen(cctx);
    m_AvailableReg = arg->m_AvailableReg;
  }

  // TODO: non collision naming, add some legit name
  std::string tempVariable = "__temp" + std::to_string(cctx.allocate()) + "__";
  cctx.noteVariable(tempVariable);
  cctx.addInstruction(std::make_shared<GetVariableInstruction>(tempVariable, m_AvailableReg - 2, cctx.getVarOffset(tempVariable)));
  cctx.addInstruction(std::make_shared<GetVariableInstruction>(tempVariable, m_AvailableReg - 1, cctx.getVarOffset(tempVariable)));
}

void UnificationNode::print(const std::string &indent)
{
  std::cout << indent << "=======[Start UnificationNode]======" << std::endl;
  m_X->print(indent + "");
  m_Y->print(indent + "");
  std::cout << indent << "=======[End UnificationNode]======" << std::endl;
}