#include "ProgramNode.hpp"

#include "../../wam_code/instruction/Instructions.hpp"

void ProgramNode::codegen(CompilationContext &cctx)
{
    // Initialize the symbol table
    for (const auto &clause : m_Clauses)
    {
        // Encode arity into the name to handle arity mismatch
        auto entry = cctx.get(clause->m_Head);
        if (!entry)
        {
            cctx.add(clause->m_Head, std::make_shared<TableEntry>(clause->m_Head));
        }
        else
        {
            entry->m_Clauses++;
        }
    }

    for (const auto &clause : m_Clauses)
    {
        clause->codegen(cctx);
    }

    // Generate the "quit" label
    cctx.addLabel("quit");
    cctx.addInstruction(std::make_shared<Backtrack>());

    cctx.getCode().updateJumpInstructions();
}

void ProgramNode::print(const std::string &indent)
{
    std::cout << indent << "=======[Start ProgramNode]======" << std::endl;
    std::cout << indent << "[m_Clauses] => " << std::endl;
    for (const auto &clause : m_Clauses)
        clause->print(indent + " ");
    std::cout << indent << "=======[End ProgramNode]======" << std::endl;
}