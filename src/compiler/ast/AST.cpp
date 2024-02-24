#include "AST.hpp"

#include <iostream>

std::string ProgramNode::codegen(CompilationContext &cctx)
{
    std::string code = "";
    // Initialize the symbol table
    for (const auto &clause : m_Clauses)
    {
        auto entry = cctx.get(clause->m_Head);
        if (!entry)
            cctx.add(clause->m_Head, new TableEntry(clause->m_Head));
        else
            entry->m_Clauses++;
    }

    for (const auto &clause : m_Clauses)
    {
        if (!code.empty())
            code += "\n";
        code += clause->codegen(cctx);
    }

    // Generate the "quit" label
    code += "\nquit: backtrack\n";
    cctx.addLabel("quit");
    cctx.addInstructions({new BacktrackInstruction()});

    return code;
}

void ProgramNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Start ProgramNode]======" << std::endl;
    std::cout << indent << "[m_Clauses] => " << std::endl;
    for (const auto &clause : m_Clauses)
        clause->print(indent + " ");
    std::cout << indent << "=======[End ProgramNode]======" << std::endl;
}

UnificationNode::UnificationNode(TermNode *x, TermNode *y)
    : m_X(x),
      m_Y(y)
{
}

std::string UnificationNode::codegen(CompilationContext &cctx)
{
    return "";
}

TermNode::TermNode(const std::string &name)
    : m_Name(name)
{
}

std::string TermNode::name()
{
    return m_Name;
}

StructNode::StructNode(const std::string &name, std::vector<TermNode *> args)
    : TermNode(name),
      m_Args(args)
{
}

std::string StructNode::codegen(CompilationContext &cctx)
{
    std::string code = "";
    if (m_IsGoal)
    {
        for (const auto &arg : m_Args)
        {
            arg->m_IsGoal = true;
            arg->m_AvailableReg = m_AvailableReg;
            code += arg->codegen(cctx) + "\n\t";
            m_AvailableReg = arg->m_AvailableReg;
        }
        code += "call " + m_Name;
        cctx.addInstructions({new CallInstruction(m_Name)});
        // Reset available registers after call
        m_AvailableReg = 1;
        return code;
    }

    cctx.addInstructions({new GetStructureInstruction(m_Name, m_AvailableReg)});
    code = "get-structure " + m_Name + " A" + std::to_string(m_AvailableReg++);
    for (const auto &arg : m_Args)
    {
        TermNode::TermType type = arg->type();
        switch (type)
        {
        case TermNode::CONST:
            cctx.addInstructions({new UnifyConstantInstruction(arg->name())});
            break;
        case TermNode::VAR:
            cctx.addInstructions({new UnifyVariableInstruction(arg->name())});
            break;
        case TermNode::STRUCT:
            break;
        }
        code += "\n\tunify " + arg->name();
    }

    return code;
}

TermNode::TermType StructNode::type()
{
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

ListNode::ListNode(const std::vector<TermNode *> &list, TermNode *tail)
    : TermNode(".")
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
}

std::string ListNode::codegen(CompilationContext &cctx)
{
    std::string code = "";
    // TODO: decide how to represent empty list
    if (m_Head.empty())
        return "get empty-list";

    code += "get-list A" + std::to_string(m_AvailableReg++);

    return code;
}

TermNode::TermType ListNode::type()
{
    return TermNode::STRUCT;
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

VarNode::VarNode(const std::string &name)
    : TermNode(name)
{
}

std::string VarNode::codegen(CompilationContext &cctx)
{
    if (!m_IsGoal)
        cctx.addInstructions({new GetVariableInstruction(m_Name, m_AvailableReg)});
    else
        cctx.addInstructions({new PutVariableInstruction(m_Name, m_AvailableReg)});
    return (m_IsGoal ? "put " : "get ") + m_Name + " A" + std::to_string(m_AvailableReg++);
}

TermNode::TermType VarNode::type()
{
    return TermNode::VAR;
}

void VarNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Start VarNode]======" << std::endl;
    std::cout << indent << "[m_Name] => " << m_Name << std::endl;
    std::cout << indent << "=======[End VarNode]======" << std::endl;
}

ConstNode::ConstNode(size_t value)
    : TermNode(std::to_string(value)),
      m_Value(value)
{
}

std::string ConstNode::codegen(CompilationContext &cctx)
{
    if (!m_IsGoal)
        cctx.addInstructions({new GetConstantInstruction(m_Name, m_AvailableReg)});
    else
        cctx.addInstructions({new PutConstantInstruction(m_Name, m_AvailableReg)});
    return (m_IsGoal ? "put" : "get") + std::string("-constant ") + m_Name + " A" + std::to_string(m_AvailableReg++);
}

TermNode::TermType ConstNode::type()
{
    return TermNode::CONST;
}

void ConstNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Start ConstNode]======" << std::endl;

    std::cout << indent << "=======[End ConstNode]======" << std::endl;
}

ClauseNode::ClauseNode(const std::string &head,
                       std::vector<TermNode *> args,
                       std::vector<GoalNode *> body)
    : m_Head(head),
      m_Args(args),
      m_Body(body)
{
}

std::string ClauseNode::codegen(CompilationContext &cctx)
{
    std::string code = "";
    TableEntry *entry = cctx.get(m_Head);
    // Generate the initial mark instruction for first clause of the predicate name
    if (!entry->m_Generated)
    {
        cctx.addLabel(m_Head);
        cctx.addInstructions({new MarkInstruction()});
        code += m_Head + ":\tmark\n";
    }
    else
    {
        std::string label = m_Head + std::to_string(entry->m_Generated);
        code += label + ":";
        cctx.addLabel(label);
    }

    ++entry->m_Generated;
    std::string retryLabel = entry->m_Generated == entry->m_Clauses ? "quit" : m_Head + std::to_string(entry->m_Generated);
    code += "\tretry-me-else " + retryLabel + "\n";
    cctx.addInstructions({new RetryMeElseInstruction(retryLabel)});

    size_t currentArgumentRegister = 1;
    for (size_t i = 0; i < m_Args.size(); i++)
    {
        m_Args[i]->m_IsGoal = false;
        m_Args[i]->m_AvailableReg = currentArgumentRegister;
        // Load the arguments into argument reigsters
        code += "\t" + m_Args[i]->codegen(cctx) + "\n";
        currentArgumentRegister = m_Args[i]->m_AvailableReg;
    }

    // All get instructions were carried out
    currentArgumentRegister = 1;

    for (size_t i = 0; i < m_Body.size(); i++)
    {
        m_Body[i]->m_AvailableReg = currentArgumentRegister;
        code += "\t" + m_Body[i]->codegen(cctx) + "\n";
        currentArgumentRegister = m_Body[i]->m_AvailableReg;
    }

    cctx.addInstructions({new ReturnInstruction()});
    return code + "\treturn\n";
}

void ClauseNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Start ClauseNode]======" << std::endl;
    std::cout << indent << "[m_Head] => " << m_Head << std::endl;
    std::cout << indent << "[m_Args] => " << std::endl;
    for (const auto &arg : m_Args)
        arg->print(indent + " ");
    std::cout << indent << "[m_Body] => " << std::endl;
    for (const auto &goal : m_Body)
        goal->print(indent + " ");
    std::cout << indent << "=======[End ClauseNode]======" << std::endl;
}