#include "StructNode.hpp"

#include <queue>

StructNode::StructNode(const std::string &name, std::vector<TermNode *> args)
    : TermNode(name),
      m_Args(args)
{

    for (const auto &arg : m_Args)
    {
        if (arg->type() == STRUCT)
        {

            m_Complex.insert({this, 0});
        }
    }
}

StructNode::~StructNode(void)
{
    for (TermNode *arg : m_Args)
        delete arg;
}

std::string StructNode::codegen(CompilationContext &cctx)
{
    std::string code = "";
    if (m_IsGoal)
    {
        if (!m_IsArg)
        {
            for (const auto &arg : m_Args)
            {
                arg->m_IsGoal = true;
                arg->m_IsArg = true;
                arg->m_AvailableReg = m_AvailableReg;
                code += arg->codegen(cctx) + "\n\t";
                m_AvailableReg = arg->m_AvailableReg;
            }
            code += "call " + m_Name;
            cctx.addInstructions({new CallInstruction(m_Name)});
            // Reset available registers after call
            m_AvailableReg = 1;
        }
        // Treat structs without arguments as constants (if they are an argument)
        // TODO: probably redundant m_IsArg check?
        else if (!m_Args.size() /*&& m_IsArg*/)
        {
            cctx.addInstructions({new PutConstantInstruction(m_Name, m_AvailableReg++)});
        }
        // Allocate space for complex structure buried inside other complex structure
        else
        {
            cctx.addInstructions({new PutStructureInstruction(m_Name, m_AvailableReg++, m_Args.size())});
            unifyRHS(cctx);
        }
        return code;
    }

    // Treat struct as a constant if it has no "arguments"
    if (m_Args.empty())
    {
        code += "get-constant " + m_Name + " A" + std::to_string(m_AvailableReg);
        cctx.addInstructions({new GetConstantInstruction(m_Name, m_AvailableReg++)});
        return code;
    }

    cctx.addInstructions({new GetStructureInstruction(m_Name, m_AvailableReg, m_Args.size())});
    code = "get-structure " + m_Name + " A" + std::to_string(m_AvailableReg++);
    unifyHead(cctx);
    return code;
}

TermNode::TermType StructNode::type()
{
    if (m_Args.empty())
    {
        return TermNode::CONST;
    }
    return TermNode::STRUCT;
}

void StructNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Start StructNode]======" << std::endl;
    std::cout << indent << "[m_Name] => " << m_Name << std::endl;
    if (!m_Args.empty())
    {
        std::cout << indent << "[m_Args] => " << std::endl;
        for (const auto &arg : m_Args)
            arg->print(indent + " ");
    }
    std::cout << indent << "=======[End StructNode]======" << std::endl;
}

void StructNode::unifyHead(CompilationContext &cctx)
{
    std::queue<std::pair<TermNode *, std::string>> terms;
    for (const auto &arg : m_Args)
    {
        TermNode::TermType type = arg->type();
        if (type == TermNode::CONST)
        {
            cctx.addInstructions({new UnifyConstantInstruction(arg->name())});
        }

        else if (type == TermNode::VAR)
        {
            // Note variable if it appears in complex structure
            cctx.noteVariable(arg->name());
            cctx.addInstructions({new UnifyVariableInstruction(arg->name(), cctx.getVarOffset(arg->name()))});
        }
        else
        {
            // Allocate extra local clause variable
            // TODO: non collision naming, add some legit name
            std::string tempVariable = "__temp" + std::to_string(cctx.allocate()) + "__";
            cctx.noteVariable(tempVariable);
            // Instead of unify-xxx (xxx = struct or list), use the unifyv for the created variable
            cctx.addInstructions({new UnifyVariableInstruction(tempVariable, cctx.getVarOffset(tempVariable))});

            // Add term to queue to be processed after all the "top level" code has been generated
            terms.push({arg, tempVariable});
        }
    }

    // Top level code has been generated
    while (!terms.empty())
    {
        auto top = terms.front();
        // Generate putv instruction to load some unneeded arg. register with the contents of the new variable
        cctx.addInstructions({new PutVariableInstruction(top.second, m_AvailableReg, cctx.getVarOffset(top.second))});
        if (top.first->type() == TermNode::STRUCT)
        {
            StructNode *arg = static_cast<StructNode *>(top.first);
            arg->m_IsGoal = true;
            arg->m_IsArg = true;
            arg->m_AvailableReg = m_AvailableReg + 1;
            cctx.addInstructions({new GetStructureInstruction(arg->name(), m_AvailableReg, arg->m_Args.size())});
            arg->unifyHead(cctx);
        }
        m_AvailableReg++;
        terms.pop();
    }
}

void StructNode::unifyRHS(CompilationContext &cctx)
{
}

bool StructNode::hasNestedComplex(void)
{
    return m_Complex.size();
}