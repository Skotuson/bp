#include "StructNode.hpp"
#include "../../wam_code/instruction/CallInstruction.hpp"
#include "../../wam_code/instruction/GetListInstruction.hpp"

#include <queue>
#include <algorithm>

StructNode::StructNode(const std::string &name, std::vector<std::shared_ptr<TermNode>> args)
    : ComplexNode(name),
      m_Args(args)
{
    for (const auto &arg : m_Args)
    {
        if (arg->type() == STRUCT || arg->type() == LIST)
        {
            ComplexNode *cn = static_cast<ComplexNode *>(arg.get());
            NestedPairing p = cn->getNestedComplex();
            // Get the information about more nested terms, increase their nested depth by one
            for (const auto &[complexNode, depth] : p)
            {
                m_Complex.insert({complexNode, depth + 1});
            }
        }
    }
    m_Complex.insert({this, 0});
}

void StructNode::codegen(CompilationContext &cctx)
{
    if (cctx.mode() == CodeGenerationMode::BODY)
    {
        unifyRHS(cctx);
        cctx.setAvailableReg(cctx.availableReg() + 1);
        return;
    }

    cctx.addInstruction(std::make_shared<GetStructureInstruction>(m_Name, cctx.availableReg(), m_Args.size()));
    unifyHead(cctx);
    cctx.setAvailableReg(cctx.availableReg() + 1);
}

TermNode::TermType StructNode::type()
{
    return TermNode::STRUCT;
}

void StructNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Start StructNode]======" << std::endl;
    std::cout << indent << "[m_Name] => " << m_Name << "/" << arity() << std::endl;
    if (!m_Args.empty())
    {
        std::cout << indent << "[m_Args] => " << std::endl;
        for (const auto &arg : m_Args)
            arg->print(indent + " ");
    }
    std::cout << indent << "=======[End StructNode]======" << std::endl;
}

size_t StructNode::arity(void)
{
    return m_Args.size();
}

std::vector<std::shared_ptr<TermNode>> StructNode::args(void)
{
    return m_Args;
}

void StructNode::unifyHead(CompilationContext &cctx)
{
    std::queue<std::pair<ComplexNode *, std::string>> terms;
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
            std::string tempVariable = cctx.generateTempVar();
            cctx.noteVariable(tempVariable);
            // Instead of unify-xxx (xxx = struct or list), use the unifyv for the created variable
            cctx.addInstruction(std::make_shared<UnifyVariableInstruction>(tempVariable, cctx.getVarOffset(tempVariable)));

            // Add term to queue to be processed after all the "top level" code has been generated
            terms.push({static_cast<ComplexNode *>(arg.get()), tempVariable});
        }
    }

    // Top level code has been generated
    while (!terms.empty())
    {
        auto top = terms.front();
        // Generate putv instruction to load some unneeded arg. register with the contents of the new variable
        cctx.addInstruction(std::make_shared<PutVariableInstruction>(top.second, cctx.availableReg(), cctx.getVarOffset(top.second)));
        auto arg = top.first;
        if (arg->type() == STRUCT)
        {
            cctx.addInstruction(std::make_shared<GetStructureInstruction>(arg->name(), cctx.availableReg(), arg->arity()));
        }
        else if (arg->type() == LIST)
        {
            cctx.addInstruction(std::make_shared<GetListInstruction>(cctx.availableReg()));
        }
        arg->unifyHead(cctx);
        terms.pop();
    }
}

void StructNode::unifyRHS(CompilationContext &cctx)
{
    if (!hasNestedComplex())
    {
        cctx.addInstruction(std::make_shared<PutStructureInstruction>(name(), cctx.availableReg(), arity()));
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
            // (c) Generate an instruction sequence as in the prior step (put-list or put-structure), but target the result to Au
            n->unifyArguments(cctx, processedComplex);
            // (d) Generate a getv to place Au in a specially allocated clause variable (as was done for nested objects in the clause head)
            std::string tempVariable = cctx.generateTempVar();
            cctx.noteVariable(tempVariable);
            cctx.addInstruction(std::make_shared<GetVariableInstruction>(tempVariable, cctx.availableReg(), cctx.getVarOffset(tempVariable)));
            processedComplex.insert({n, tempVariable});
            // (e) Repeat the above process for the next most nested component, expect that for components that refer to nested structures that have already been processed
        }
    }
}

void StructNode::unifyArguments(CompilationContext &cctx, ProcessedComplex &processedComplex)
{
    // (c) Generate an instruction sequence as in the prior step (put-list or put-structure), but target the result to Au
    cctx.addInstruction(std::make_shared<PutStructureInstruction>(name(), cctx.availableReg(), arity()));
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

        else
        {
            // Use a unifyv instruction with the offset of the clause variable into which they were compiled earlier:
            std::string var = processedComplex[arg.get()];
            cctx.addInstruction(std::make_shared<UnifyVariableInstruction>(var, cctx.getVarOffset(var)));
        }
    }
}