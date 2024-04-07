#include "ListNode.hpp"

#include "StructNode.hpp"

#include <queue>
#include <algorithm>

ListNode::ListNode(const std::vector<TermNode *> &list, TermNode *tail)
    : ComplexNode("[]")
{
    if (tail)
    {
        m_Head = list;
        m_Tail = tail;
    }
    else if (!list.empty())
    {
        {
            m_Head = {list.front()};
            m_Tail = new ListNode({list.begin() + 1, list.end()});
        }
    }

    if (type() == LIST)
    {
        std::vector<TermNode *> args = m_Head;
        if (tail)
        {
            args.push_back(tail);
        }
        for (const auto &arg : args)
        {
            if (arg->type() == STRUCT || arg->type() == LIST)
            {
                ComplexNode *cn = static_cast<ComplexNode *>(arg);
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
}

ListNode::~ListNode(void)
{
    // for (TermNode *el : m_Head)
    //     delete el;
    // delete m_Tail;
}

void ListNode::codegen(CompilationContext &cctx)
{
    std::string code = "";
    if (m_Head.empty())
    {
        if (!m_IsGoal)
        {
            cctx.addInstruction(std::make_shared<GetConstantInstruction>("[]", m_AvailableReg++));
        }
        else
        {
            cctx.addInstruction(std::make_shared<PutConstantInstruction>("[]", m_AvailableReg++));
        }
        return;
    }

    if (m_IsGoal)
    {
        unifyRHS(cctx);
        m_AvailableReg++;
        return;
    }

    cctx.addInstruction(std::make_shared<GetListInstruction>(m_Name, m_AvailableReg));
    unifyHead(cctx);
    m_AvailableReg++;
}

TermNode::TermType ListNode::type()
{
    if (m_Head.empty())
    {
        return CONST;
    }
    return LIST;
}

void ListNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Start ListNode]======" << std::endl;
    if (!m_Head.empty())
    {
        std::cout << indent << "[m_Head] => " << std::endl;
        for (const auto &term : m_Head)
            term->print(indent + " ");
    }
    if (m_Tail)
    {
        std::cout << indent << "[m_Tail] => " << std::endl;
        m_Tail->print(indent + " ");
    }
    std::cout << indent << "=======[End ListNode]======" << std::endl;
}

size_t ListNode::arity(void)
{
    return m_Head.size() + 1;
}

void ListNode::unifyHead(CompilationContext &cctx)
{
    std::queue<std::pair<ComplexNode *, std::string>> terms;
    // Generate code for car and cdr of the list.
    std::vector<TermNode *> args = m_Head;
    args.push_back(m_Tail);
    for (const auto &arg : args)
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
            terms.push({static_cast<ComplexNode *>(arg), tempVariable});
        }
    }
    // Top level code has been generated
    while (!terms.empty())
    {
        auto top = terms.front();
        // Generate putv instruction to load some unneeded arg. register with the contents of the new variable
        cctx.addInstruction(std::make_shared<PutVariableInstruction>(top.second, m_AvailableReg, cctx.getVarOffset(top.second)));
        auto arg = top.first;
        arg->m_IsGoal = true;
        arg->m_IsArg = true;
        arg->m_AvailableReg = m_AvailableReg;
        if (arg->type() == STRUCT)
        {
            cctx.addInstruction(std::make_shared<GetStructureInstruction>(arg->name(), m_AvailableReg, arg->arity()));
        }
        else if (arg->type() == LIST)
        {
            cctx.addInstruction(std::make_shared<GetListInstruction>(arg->name(), m_AvailableReg));
        }
        arg->unifyHead(cctx);
        terms.pop();
    }
}

void ListNode::unifyRHS(CompilationContext &cctx)
{
    if (!hasNestedComplex())
    {
        cctx.addInstruction(std::make_shared<PutListInstruction>(name(), m_AvailableReg));
        std::vector<TermNode *> args = m_Head;
        args.push_back(m_Tail);
        for (const auto &arg : args)
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

void ListNode::unifyArguments(CompilationContext &cctx, ProcessedComplex &processedComplex)
{
    // (c) Generate an instruction sequence as in the prior step (put-list or put-structure), but target the result to Au
    cctx.addInstruction(std::make_shared<PutListInstruction>(name(), m_AvailableReg));
    std::vector<TermNode *> args = m_Head;
    args.push_back(m_Tail);
    for (const auto &arg : args)
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
            std::string var = processedComplex[arg];
            cctx.addInstruction(std::make_shared<UnifyVariableInstruction>(var, cctx.getVarOffset(var)));
        }
    }
}