#include "ProgramNode.hpp"

#include "../../wam_code/instruction/Instructions.hpp"

void ProgramNode::codegen(CompilationContext &cctx)
{
    // Initialize the symbol table
    for (const auto &clause : m_Clauses)
    {
        // Encode arity into the name to handle arity mismatch
        auto entry = cctx.get(clause->head());
        if (!entry)
        {
            cctx.add(clause->head(), std::make_shared<TableEntry>());
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
    cctx.addLabel("__quit");
    cctx.addInstruction(std::make_shared<Backtrack>());

    // Update the addresses of branch instructions
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

void ProgramNode::addClause(std::shared_ptr<ClauseNode> clause)
{
    m_Clauses.push_back(clause);
}