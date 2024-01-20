#include "LexerTest.hpp"
#include "../../../src/compiler/lexer/Lexer.hpp"

#include <cassert>
#include <sstream>
#include <iostream>

void LexerTest::test ( void ) {    
    { 
        std::istringstream iss ( "a b" );
        Lexer lex ( iss );
        assert ( lex.get() == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "a" );

        assert ( lex.get() == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "b" );

        assert ( lex.get() == TOK_EOF ); 
    }

    {
        std::istringstream iss ("a Xab ab\n\t abcXYZsitrX aX XyZ");
        Lexer lex ( iss );
        assert ( lex.get() == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "a" );

        assert ( lex.get() == TOK_VAR );
        assert ( lex.identifier() == "Xab" );

        assert ( lex.get() == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "ab" );

        assert ( lex.get() == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "abcXYZsitrX" );

        assert ( lex.get() == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "aX" );

        assert ( lex.get() == TOK_VAR );
        assert ( lex.identifier() == "XyZ" );
    }

    {
        std::istringstream iss ("bigger(elephant, horse).");
        Lexer lex ( iss );
        assert ( lex.get() == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "bigger" );

        assert ( lex.get() == TOK_LPAR );

        assert ( lex.get() == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "elephant" );

        assert ( lex.get() == TOK_COMMA );

        assert ( lex.get() == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "horse" );

        assert ( lex.get() == TOK_RPAR );

        assert ( lex.get() == TOK_PERIOD );
    }

    {
        std::istringstream iss ("is_bigger(X, Y) :- bigger(X, Z), is_bigger(Z, Y).");
        Lexer lex ( iss );
        assert ( lex.get() == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "is_bigger" );

        assert ( lex.get() == TOK_LPAR );

        assert ( lex.get() == TOK_VAR );
        assert ( lex.identifier() == "X" );

        assert ( lex.get() == TOK_COMMA );

        assert ( lex.get() == TOK_VAR );
        assert ( lex.identifier() == "Y" );

        assert ( lex.get() == TOK_RPAR );

        assert ( lex.get() == TOK_IF );

        assert ( lex.get() == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "bigger" );

        assert ( lex.get() == TOK_LPAR );

        assert ( lex.get() == TOK_VAR );
        assert ( lex.identifier() == "X" );

        assert ( lex.get() == TOK_COMMA );

        assert ( lex.get() == TOK_VAR );
        assert ( lex.identifier() == "Z" );

        assert ( lex.get() == TOK_RPAR );

        assert ( lex.get() == TOK_COMMA );

        assert ( lex.get() == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "is_bigger" );

        assert ( lex.get() == TOK_LPAR );

        assert ( lex.get() == TOK_VAR );
        assert ( lex.identifier() == "Z" );

        assert ( lex.get() == TOK_COMMA );

        assert ( lex.get() == TOK_VAR );
        assert ( lex.identifier() == "Y" );

        assert ( lex.get() == TOK_RPAR );

        assert ( lex.get() == TOK_PERIOD );
    }

    {
        std::istringstream iss ("a(b, c, [H,T])");
        Lexer lex ( iss );
        assert ( lex.get() == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "a" );

        assert ( lex.get() == TOK_LPAR );

        assert ( lex.get() == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "b" );

        assert ( lex.get() == TOK_COMMA );

        assert ( lex.get() == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "c" );

        assert ( lex.get() == TOK_COMMA );

        assert ( lex.get() == TOK_LSPAR );

        assert ( lex.get() == TOK_VAR );
        assert ( lex.identifier() == "H" );

        assert ( lex.get() == TOK_COMMA );

        assert ( lex.get() == TOK_VAR );
        assert ( lex.identifier() == "T" );

        assert ( lex.get() == TOK_RSPAR );
        
        assert ( lex.get() == TOK_RPAR );
    }

    {
        std::istringstream iss ("  \t    \n\r\t            \n");
        Lexer lex ( iss );
        assert ( lex.get() == TOK_EOF );
    }
}
