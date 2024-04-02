#include "CompilationContext.hpp"

CompilationContext::~CompilationContext(void)
{
    for (const auto &entry : m_SymbolTable)
    {
        delete entry.second;
    }
}

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

void CompilationContext::addInstruction(Instruction *instr)
{
    m_GeneratedCode.addInstructions({instr});
}

void CompilationContext::addInstructions(const std::vector<Instruction *> &instructions)
{
    m_GeneratedCode.addInstructions(instructions);
}

void CompilationContext::addLabel(const Label &label)
{
    m_GeneratedCode.addLabel(label);
}

size_t CompilationContext::getLabelAddress(const Label &label)
{
    return m_GeneratedCode.m_LabelToAddress[label];
}

WAMCode CompilationContext::code()
{
    return m_GeneratedCode;
}

WAMCode &CompilationContext::getCode()
{
    return m_GeneratedCode;
}

void CompilationContext::addVariable(const std::string &variable)
{
    m_GeneratedCode.addVariable({variable, getVarOffset(variable)});
}

size_t CompilationContext::allocate(void)
{
    return m_Variables.size();
}

void CompilationContext::noteVariable(const std::string &variable)
{
    if (m_Variables.count(variable))
        return;
    m_Variables.insert({variable, m_Variables.size()});
}

size_t CompilationContext::getVarOffset(const std::string &variable)
{
    return m_Variables[variable];
}

void CompilationContext::resetVariables(void)
{
    m_Variables.clear();
}