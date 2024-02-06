#include "SymbolTable.hpp"

void SymbolTable::add(const std::string &symbol, TableEntry *entry)
{
    if (!m_Table.count(symbol))
        m_Table.insert({symbol, entry});
}

TableEntry *SymbolTable::get(const std::string &symbol)
{
    auto it = m_Table.find(symbol);
    if (it != m_Table.end())
        return it->second;
    return nullptr;
}