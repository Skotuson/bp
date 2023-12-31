#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <unordered_map>

struct TableEntry {
    
    bool hasCodeGenerated = false;
};

class SymbolTable {
    public:
        void       add ( const std::string & symbol, 
                         const TableEntry & entry );
        TableEntry get ( const std::string & symbol );

    private:
        std::unordered_map<std::string, TableEntry> m_Table;
};

#endif