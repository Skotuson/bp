#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <map>
#include <string>
#include <unordered_map>

using Label = std::string;

struct TableEntry
{
    TableEntry(const std::string &name)
        : m_Name(name)
    {
    }

    std::string m_Name;
    size_t m_Generated = 0;
    size_t m_Clauses = 1;
};

class CompilationContext
{
public:
    void add(const std::string &symbol,
             TableEntry *entry);
    TableEntry *get(const std::string &symbol);

private:
    std::unordered_map<std::string, TableEntry *> m_SymbolTable;
    size_t m_Line = 0;
    std::map<Label, size_t> labels;
};

#endif