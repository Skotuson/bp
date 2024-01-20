#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <unordered_map>

struct TableEntry {
    TableEntry ( const std::string & name )
    : m_Name ( name )
    {}

    std::string m_Name;
    size_t      m_Clauses = 1;
};

class SymbolTable {
    public:
        void         add ( const std::string & symbol, 
                           TableEntry        * entry );
        TableEntry * get ( const std::string & symbol );

    private:
        std::unordered_map<std::string, TableEntry *> m_Table;
};

#endif