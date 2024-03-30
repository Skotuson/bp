#include "StructNode.hpp"

#include <queue>
#include <algorithm>

StructNode::StructNode(const std::string &name, std::vector<TermNode *> args)
    : TermNode(name),
      m_Args(args)
{
    for (const auto &arg : m_Args)
    {
        if (arg->type() == STRUCT)
        {
            StructNode *sn = static_cast<StructNode *>(arg);
            NestedPairing p = sn->getNestedComplex();
            // Get the information about more nested terms, increase their nested depth by one
            for (const auto &[structureNode, depth] : p)
            {
                m_Complex.insert({structureNode, depth + 1});
            }
            // m_Complex.insert({sn, 0});
        }
    }
    m_Complex.insert({this, 0});
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
            cctx.addInstruction(new CallInstruction(m_Name));
            // Reset available registers after call
            m_AvailableReg = 1;
        }
        // Treat structs without arguments as constants (if they are an argument)
        else if (!m_Args.size())
        {
            cctx.addInstruction(new PutConstantInstruction(m_Name, m_AvailableReg++));
        }
        // Allocate space for complex structure buried inside other complex structure
        else
        {
            unifyRHS(cctx);
            m_AvailableReg++;
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
    unifyHead(cctx);
    m_AvailableReg++;
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

    for (const auto &[strct, depth] : m_Complex)
    {
        std::cout << indent << strct->name() << " " << depth << std::endl;
    }

    std::cout << indent << "=======[End StructNode]======" << std::endl;
}

size_t StructNode::arity(void)
{
    return m_Args.size();
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
            arg->m_AvailableReg = m_AvailableReg;
            cctx.addInstructions({new GetStructureInstruction(arg->name(), m_AvailableReg, arg->m_Args.size())});
            arg->unifyHead(cctx);
        }
        // m_AvailableReg++;
        terms.pop();
    }
}

void StructNode::unifyRHS(CompilationContext &cctx)
{
    if (!hasNestedComplex())
    {
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
        }
    }

    // hasNestedComplex()
    else
    {
        std::vector<StructNode *> nested;
        std::map<Node *, std::string> processedComplex;
        for (const auto &c : m_Complex)
        {
            nested.push_back(c.first);
        }

        // Sort the nested structures from the most nested to the least nested
        std::sort(nested.begin(), nested.end(), [&](StructNode *&a, StructNode *&b)
                  { return m_Complex[a] > m_Complex[b]; });

        // (a) Take the most deeply nested such component
        for (const auto &n : nested)
        {
            // (b) Select a currently unsued argument register Au - the m_AvailableReg show the next available register
            // (c) Generate an instruction sequence as in the prior step (put-list or put-structure), but target the result to Au
            cctx.addInstruction(new PutStructureInstruction(n->name(), m_AvailableReg, n->arity()));
            for (const auto &arg : n->m_Args)
            {
                TermType type = arg->type();
                if (type == TermNode::CONST)
                {
                    cctx.addInstruction(new UnifyConstantInstruction(arg->name()));
                }

                else if (type == TermNode::VAR)
                {
                    // Note variable if it appears in complex structure
                    cctx.noteVariable(arg->name());
                    cctx.addInstruction(new UnifyVariableInstruction(arg->name(), cctx.getVarOffset(arg->name())));
                }

                else if (type == TermNode::STRUCT)
                {
                    // Use a unifyv instructionwith the offset of the clause variable into which they were compiled earlier:
                    std::string var = processedComplex[arg];
                    cctx.addInstruction(new UnifyVariableInstruction(var, cctx.getVarOffset(var)));
                }
            }
            // (d) Generate a getv to place Au in a specially allocated clause variable (as was done for nested objects in the clause head)
            // TODO: non collision naming, add some legit name, make a function to remove duplicity
            std::string tempVariable = "__temp" + std::to_string(cctx.allocate()) + "__";
            cctx.noteVariable(tempVariable);
            cctx.addInstruction(new GetVariableInstruction(tempVariable, m_AvailableReg, cctx.getVarOffset(tempVariable)));
            processedComplex.insert({n, tempVariable});
            // (e) Repeat the above process for the next most neste component, expect that for components that refer to nested structures that have already been processed
        }
    }
}

bool StructNode::hasNestedComplex(void)
{
    return m_Complex.size();
}

NestedPairing StructNode::getNestedComplex(void)
{
    return m_Complex;
}