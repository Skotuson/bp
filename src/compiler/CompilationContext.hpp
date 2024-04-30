#pragma once

#include <set>
#include <string>
#include <memory>
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
    void addInstruction(std::shared_ptr<Instruction> instr);
    void addInstructions(const std::vector<std::shared_ptr<Instruction>> &instructions);
    void addLabel(const Label &label);
    size_t getLabelAddress(const Label &label);
    WAMCode code(void);
    WAMCode &getCode(void);

    std::string generateTempVar(void);

    void addVariable(const std::string &variable);

    size_t allocate(void);
    void noteVariable(const std::string &variable);
    size_t getVarOffset(const std::string &variable);
    void resetVariables(void);

private:
    std::unordered_map<std::string, TableEntry *> m_SymbolTable;
    WAMCode m_GeneratedCode;

    // Used in Clause-Level compilation
    std::map<std::string, size_t> m_Variables;
};