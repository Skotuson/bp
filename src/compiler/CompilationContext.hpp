#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <set>
#include <string>
#include <unordered_map>

#include "../wam_code/WAMCode.hpp"

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
    ~CompilationContext(void);

    void add(const std::string &symbol,
             TableEntry *entry);
    TableEntry *get(const std::string &symbol);
    void addInstructions(const std::vector<Instruction *> &instructions);
    void addLabel(const Label &label);
    size_t getLabelAddress(const Label &label);
    WAMCode code(void);
    WAMCode &getCode(void);

    size_t &allocate(void);
    void noteVariable(const std::string &variable);
    size_t getVarOffset(const std::string &variable);
    void resetVariables(void);

private:
    std::unordered_map<std::string, TableEntry *> m_SymbolTable;
    WAMCode m_GeneratedCode;

    // Used in Clause-Level compilation
    size_t m_Allocate = 0;
    std::map<std::string, size_t> m_Variables;
};

#endif