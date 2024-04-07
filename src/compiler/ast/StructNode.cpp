#include "StructNode.hpp"

#include <queue>
#include <algorithm>

StructNode::StructNode(const std::string &name, std::vector<TermNode *> args)
    : ComplexNode(name),
      m_Args(args)
{
    for (const auto &arg : m_Args)
    {
        if (arg->type() == STRUCT)
        {
            StructNode *sn = static_cast<StructNode *>(arg);
            NestedPairing p = sn->getNestedComplex();
            // Get the information about more nested terms, increase their nested depth by one
            for (const auto &[complexNode, depth] : p)
            {
                m_Complex.insert({complexNode, depth + 1});
            }
        }
        /*TODO: arg->type() == list*/
    }
    m_Complex.insert({this, 0});
}

StructNode::~StructNode(void)
{
    for (TermNode *arg : m_Args)
        delete arg;
}

void StructNode::codegen(CompilationContext &cctx)
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
                arg->codegen(cctx);
                m_AvailableReg = arg->m_AvailableReg;
            }
            std::string callName = m_Name + "/" + std::to_string(m_Args.size());
            cctx.addInstruction(std::make_shared<CallInstruction>(callName));
            // Reset available registers after call
            m_AvailableReg = 1;
        }
        // Treat structs without arguments as constants (if they are an argument)
        else if (m_Args.empty())
        {
            cctx.addInstruction(std::make_shared<PutConstantInstruction>(m_Name, m_AvailableReg++));
        }
        // Allocate space for complex structure buried inside other complex structure
        else
        {
            unifyRHS(cctx);
            m_AvailableReg++;
        }
        return;
    }

    // Treat struct as a constant if it has no "arguments"
    if (m_Args.empty())
    {
        code += "get-constant " + m_Name + " A" + std::to_string(m_AvailableReg);
        cctx.addInstruction(std::make_shared<GetConstantInstruction>(m_Name, m_AvailableReg++));
        return;
    }

    cctx.addInstruction(std::make_shared<GetStructureInstruction>(m_Name, m_AvailableReg, m_Args.size()));
    unifyHead(cctx);
    m_AvailableReg++;
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
            cctx.addInstruction(std::make_shared<UnifyConstantInstruction>(arg->name()));
        }

        else if (type == TermNode::VAR)
        {
            // Note variable if it appears in complex structure
            cctx.noteVariable(arg->name());
            cctx.addVariable(arg->name());
            cctx.addInstruction(std::make_shared<UnifyVariableInstruction>(arg->name(), cctx.getVarOffset(arg->name())));
        }
        else
        {
            // Allocate extra local clause variable
            // TODO: non collision naming, add some legit name
            std::string tempVariable = "__temp" + std::to_string(cctx.allocate()) + "__";
            cctx.noteVariable(tempVariable);
            // Instead of unify-xxx (xxx = struct or list), use the unifyv for the created variable
            cctx.addInstruction(std::make_shared<UnifyVariableInstruction>(tempVariable, cctx.getVarOffset(tempVariable)));

            // Add term to queue to be processed after all the "top level" code has been generated
            terms.push({arg, tempVariable});
        }
    }

    // Top level code has been generated
    while (!terms.empty())
    {
        auto top = terms.front();
        // Generate putv instruction to load some unneeded arg. register with the contents of the new variable
        cctx.addInstruction(std::make_shared<PutVariableInstruction>(top.second, m_AvailableReg, cctx.getVarOffset(top.second)));
        if (top.first->type() == TermNode::STRUCT)
        {
            StructNode *arg = static_cast<StructNode *>(top.first);
            arg->m_IsGoal = true;
            arg->m_IsArg = true;
            arg->m_AvailableReg = m_AvailableReg;
            cctx.addInstruction(std::make_shared<GetStructureInstruction>(arg->name(), m_AvailableReg, arg->m_Args.size()));
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
        cctx.addInstruction(std::make_shared<PutStructureInstruction>(name(), m_AvailableReg, arity()));
        for (const auto &arg : m_Args)
        {
            TermNode::TermType type = arg->type();
            if (type == TermNode::CONST)
            {
                cctx.addInstruction(std::make_shared<UnifyConstantInstruction>(arg->name()));
            }

            else if (type == TermNode::VAR)
            {
                // Note variable if it appears in complex structure
                cctx.noteVariable(arg->name());
                cctx.addVariable(arg->name());
                cctx.addInstruction(std::make_shared<UnifyVariableInstruction>(arg->name(), cctx.getVarOffset(arg->name())));
            }
        }
    }

    else // hasNestedComplex()
    {
        std::vector<ComplexNode *> nested;
        ProcessedComplex processedComplex;
        for (const auto &c : m_Complex)
        {
            nested.push_back(c.first);
        }

        // Sort the nested structures from the most nested to the least nested
        std::sort(nested.begin(), nested.end(), [&](ComplexNode *&a, ComplexNode *&b)
                  { return m_Complex[a] > m_Complex[b]; });

        // (a) Take the most deeply nested such component
        for (const auto &n : nested)
        {
            // (b) Select a currently unsued argument register Au - the m_AvailableReg variable is the next available register
            n->m_AvailableReg = m_AvailableReg;
            // (c) Generate an instruction sequence as in the prior step (put-list or put-structure), but target the result to Au
            n->unifyArguments(cctx, processedComplex);
            // (d) Generate a getv to place Au in a specially allocated clause variable (as was done for nested objects in the clause head)
            // TODO: non collision naming, add some legit name, make a function to remove duplicity
            std::string tempVariable = "__temp" + std::to_string(cctx.allocate()) + "__";
            cctx.noteVariable(tempVariable);
            cctx.addInstruction(std::make_shared<GetVariableInstruction>(tempVariable, m_AvailableReg, cctx.getVarOffset(tempVariable)));
            processedComplex.insert({n, tempVariable});
            // (e) Repeat the above process for the next most nested component, expect that for components that refer to nested structures that have already been processed
        }
    }
}

void StructNode::unifyArguments(CompilationContext &cctx, ProcessedComplex &processedComplex)
{
    // (c) Generate an instruction sequence as in the prior step (put-list or put-structure), but target the result to Au
    cctx.addInstruction(std::make_shared<PutStructureInstruction>(name(), m_AvailableReg, arity()));
    for (const auto &arg : m_Args)
    {
        TermType type = arg->type();
        if (type == TermNode::CONST)
        {
            cctx.addInstruction(std::make_shared<UnifyConstantInstruction>(arg->name()));
        }

        else if (type == TermNode::VAR)
        {
            // Note variable if it appears in complex structure
            cctx.noteVariable(arg->name());
            cctx.addVariable(arg->name());
            cctx.addInstruction(std::make_shared<UnifyVariableInstruction>(arg->name(), cctx.getVarOffset(arg->name())));
        }

        else if (type == TermNode::STRUCT)
        {
            // Use a unifyv instruction with the offset of the clause variable into which they were compiled earlier:
            std::string var = processedComplex[arg];
            cctx.addInstruction(std::make_shared<UnifyVariableInstruction>(var, cctx.getVarOffset(var)));
        }
    }
}