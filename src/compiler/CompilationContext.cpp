#include "CompilationContext.hpp"

void CompilationContext::add(const std::string &symbol, TableEntry *entry)
{
    if (!m_SymbolTable.count(symbol))
        m_SymbolTable.insert({symbol, entry});
}

TableEntry *CompilationContext::get(const std::string &symbol)
{
    auto it = m_SymbolTable.find(symbol);
    if (it != m_SymbolTable.end())
        return it->second;
    return nullptr;
}

void CompilationContext::addInstructions(std::vector<Instruction*> & instructions) {
    generatedCode.addInstructions(instructions);
}

void CompilationContext::addLabel(const Label & label)
{
    generatedCode.addLabel(label);
}