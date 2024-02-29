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

void CompilationContext::addInstructions(const std::vector<Instruction*> & instructions) {
    m_GeneratedCode.addInstructions(instructions);
}

void CompilationContext::addLabel(const Label & label)
{
    m_GeneratedCode.addLabel(label);
}

size_t CompilationContext::getLabelAddress(const Label & label)
{
    return m_GeneratedCode.m_LabelToAddress[label];
}

WAMCode CompilationContext::code()
{
    return m_GeneratedCode;
}