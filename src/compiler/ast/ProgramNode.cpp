#include "ProgramNode.hpp"

std::string ProgramNode::codegen(CompilationContext &cctx)
{
    std::string code = "";
    // Initialize the symbol table
    for (const auto &clause : m_Clauses)
    {
        // Encode arity into the name to handle arity mismatch
        auto entry = cctx.get(clause->m_Head);
        if (!entry)
        {
            cctx.add(clause->m_Head, new TableEntry(clause->m_Head));
        }
        else
        {
            entry->m_Clauses++;
        }
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
    cctx.addInstruction(std::make_shared<BacktrackInstruction>());

    cctx.getCode().updateJumpInstructions();

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