#include "ParserTest.hpp"
#include "../../../src/compiler/lexer/Lexer.hpp"
#include "../../../src/compiler/parser/Parser.hpp"

#include <cassert>
#include <sstream>
#include <iostream>

void ParserTest::test ( void ) {
    {
        std::istringstream iss ( "bigger(elephant, mouse)." );
        Lexer lex ( iss );
        Parser parser ( lex );
        assert ( parser.parse() );
    }

    {
        std::istringstream iss 
        ( "is_bigger(X, Y) :- bigger(X, Y).\n"
          "is_bigger(X, Y) :- bigger(X, Z), is_bigger(Z, Y)." );
        Lexer lex ( iss );
        Parser parser ( lex );
        assert ( parser.parse() );
    }

    {
        std::istringstream iss 
        ( "bigger(whale, _).\n"
          "prolog_is_beautiful.\n" 
          "is_smaller(X, Y) :- is_bigger(Y, X).\n"
          "aunt(Aunt, Child) :-\n"
          "\tsister(Aunt, Parent),"
          "\t parent(Parent, Child).");
        Lexer lex ( iss );
        Parser parser ( lex );
        assert ( parser.parse() );
    }

    {
        std::istringstream iss 
        ( "vodorovna(usecka(bod(a,b), bod(b, c))).\n");
        Lexer lex ( iss );
        Parser parser ( lex );
        assert ( parser.parse() );
    }

    std::cout << "Parser Test suite completed :)" << std::endl;
}
