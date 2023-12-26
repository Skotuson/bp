#include "LexerTest.hpp"
#include "../../../src/compiler/lexer/Lexer.hpp"

#include <cassert>
#include <sstream>
#include <iostream>

void LexerTest::test ( void ) {    
    { 
        std::istringstream iss ( "a b" );
        Lexer lex ( iss );
        assert ( lex.getToken() == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "a" );

        assert ( lex.getToken() == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "b" );

        assert ( lex.getToken() == TOK_EOF ); 
    }

    {
        std::istringstream iss ("a Xab ab\n\t abcXYZsitrX aX XyZ");
        Lexer lex ( iss );
        assert ( lex.getToken()   == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "a" );

        assert ( lex.getToken()   == TOK_ATOM_VAR );
        assert ( lex.identifier() == "Xab" );

        assert ( lex.getToken()   == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "ab" );

        assert ( lex.getToken()   == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "abcXYZsitrX" );

        assert ( lex.getToken()   == TOK_ATOM_LOWER );
        assert ( lex.identifier() == "aX" );

        assert ( lex.getToken()   == TOK_ATOM_VAR );
        assert ( lex.identifier() == "XyZ" );
    }

    {
        std::istringstream iss ("  \t    \n\r\t            \n");
        Lexer lex ( iss );
        assert ( lex.getToken() == TOK_EOF );
    }
}
