#ifndef COMPILER_H
#define COMPILER_H

#include <iostream>
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "SymbolTable.hpp"

class Compiler {
    public:
        Compiler ( std::istream & is, 
                   std::ostream & os );

        void compile ( void );
    private:
        SymbolTable m_Table;
        Parser m_Parser = Parser( Lexer ( std::cin ) );
        std::ostream & m_Out;
};

#endif