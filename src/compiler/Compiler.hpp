#ifndef COMPILER_H
#define COMPILER_H

#include <iostream>
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"

class Compiler {
    public:
        Compiler ( std::istream & is, 
                   std::ostream & os );

        void compile ( void );
    private:
        Parser m_Parser = Parser( Lexer ( std::cin ) );
};

#endif