#include "../../doctest.h"

#include "../../../src/compiler/lexer/Lexer.hpp"

#include <sstream>
#include <iostream>

TEST_CASE( "Lex expression: a b" ) {
    std::istringstream iss ( "a b" );
    Lexer lex ( iss );
    CHECK ( lex.get() == TOK_ATOM_LOWER );
    CHECK ( lex.identifier() == "a" );
    CHECK ( lex.get() == TOK_ATOM_LOWER );
    CHECK ( lex.identifier() == "b" );
    CHECK ( lex.get() == TOK_EOF ); 
}

TEST_CASE ( "Lex Expression: a Xab ab abcXYZsitrX aX XyZ" ) {
    std::istringstream iss ("a Xab ab\n\t abcXYZsitrX aX XyZ");
    Lexer lex ( iss );
    CHECK ( lex.get() == TOK_ATOM_LOWER );
    CHECK ( lex.identifier() == "a" );
    CHECK ( lex.get() == TOK_VAR );
    CHECK ( lex.identifier() == "Xab" );
    CHECK ( lex.get() == TOK_ATOM_LOWER );
    CHECK ( lex.identifier() == "ab" );
    CHECK ( lex.get() == TOK_ATOM_LOWER );
    CHECK ( lex.identifier() == "abcXYZsitrX" );
    CHECK ( lex.get() == TOK_ATOM_LOWER );
    CHECK ( lex.identifier() == "aX" );
    CHECK ( lex.get() == TOK_VAR );
    CHECK ( lex.identifier() == "XyZ" );
}

TEST_CASE ( "Lex Expression: bigger(elephant, horse)." ) {
    std::istringstream iss ("bigger(elephant, horse).");
    Lexer lex ( iss );
    CHECK ( lex.get() == TOK_ATOM_LOWER );
    CHECK ( lex.identifier() == "bigger" );
    CHECK ( lex.get() == TOK_LPAR );
    CHECK ( lex.get() == TOK_ATOM_LOWER );
    CHECK ( lex.identifier() == "elephant" );
    CHECK ( lex.get() == TOK_COMMA );
    CHECK ( lex.get() == TOK_ATOM_LOWER );
    CHECK ( lex.identifier() == "horse" );
    CHECK ( lex.get() == TOK_RPAR );
    CHECK ( lex.get() == TOK_PERIOD );
}

TEST_CASE ( "Lex Expression: is_bigger(X, Y) :- bigger(X, Z), is_bigger(Z, Y)." ) {
    std::istringstream iss ("is_bigger(X, Y) :- bigger(X, Z), is_bigger(Z, Y).");
    Lexer lex ( iss );
    CHECK ( lex.get() == TOK_ATOM_LOWER );
    CHECK ( lex.identifier() == "is_bigger" );
    CHECK ( lex.get() == TOK_LPAR );
    CHECK ( lex.get() == TOK_VAR );
    CHECK ( lex.identifier() == "X" );
    CHECK ( lex.get() == TOK_COMMA );
    CHECK ( lex.get() == TOK_VAR );
    CHECK ( lex.identifier() == "Y" );
    CHECK ( lex.get() == TOK_RPAR );
    CHECK ( lex.get() == TOK_IF );
    CHECK ( lex.get() == TOK_ATOM_LOWER );
    CHECK ( lex.identifier() == "bigger" );
    CHECK ( lex.get() == TOK_LPAR );
    CHECK ( lex.get() == TOK_VAR );
    CHECK ( lex.identifier() == "X" );
    CHECK ( lex.get() == TOK_COMMA );
    CHECK ( lex.get() == TOK_VAR );
    CHECK ( lex.identifier() == "Z" );
    CHECK ( lex.get() == TOK_RPAR );
    CHECK ( lex.get() == TOK_COMMA );
    CHECK ( lex.get() == TOK_ATOM_LOWER );
    CHECK ( lex.identifier() == "is_bigger" );
    CHECK ( lex.get() == TOK_LPAR );
    CHECK ( lex.get() == TOK_VAR );
    CHECK ( lex.identifier() == "Z" );
    CHECK ( lex.get() == TOK_COMMA );
    CHECK ( lex.get() == TOK_VAR );
    CHECK ( lex.identifier() == "Y" );
    CHECK ( lex.get() == TOK_RPAR );
    CHECK ( lex.get() == TOK_PERIOD );
}

TEST_CASE ( "Lex Expression: a(b, c, [H|T])" ) {
    std::istringstream iss ("a(b, c, [H|T])");
    Lexer lex ( iss );
    CHECK ( lex.get() == TOK_ATOM_LOWER );
    CHECK ( lex.identifier() == "a" );
    CHECK ( lex.get() == TOK_LPAR );
    CHECK ( lex.get() == TOK_ATOM_LOWER );
    CHECK ( lex.identifier() == "b" );
    CHECK ( lex.get() == TOK_COMMA );
    CHECK ( lex.get() == TOK_ATOM_LOWER );
    CHECK ( lex.identifier() == "c" );
    CHECK ( lex.get() == TOK_COMMA );
    CHECK ( lex.get() == TOK_LSPAR );
    CHECK ( lex.get() == TOK_VAR );
    CHECK ( lex.identifier() == "H" );
    CHECK ( lex.get() == TOK_PIPE );
    CHECK ( lex.get() == TOK_VAR );
    CHECK ( lex.identifier() == "T" );
    CHECK ( lex.get() == TOK_RSPAR );
    CHECK ( lex.get() == TOK_RPAR );
}

TEST_CASE ( "Lex Expression: Random whitespace" ) {
    std::istringstream iss ("  \t    \n\r\t            \n");
    Lexer lex ( iss );
    CHECK ( lex.get() == TOK_EOF );
}