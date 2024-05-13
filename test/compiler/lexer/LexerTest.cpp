#include "../../doctest.h"

#include "../../../src/compiler/lexer/Lexer.hpp"

#include <sstream>
#include <iostream>

TEST_CASE("Lexer test suite")
{

    SUBCASE("Lex expression: a b")
    {
        std::istringstream iss("a b");
        Lexer lex(iss);
        CHECK(lex.get() == TOK_ATOM_LOWER);
        CHECK(lex.identifier() == "a");
        CHECK(lex.get() == TOK_ATOM_LOWER);
        CHECK(lex.identifier() == "b");
        CHECK(lex.get() == TOK_EOF);
    }

    SUBCASE("Lex expression: A13")
    {
        std::istringstream iss("A13");
        Lexer lex(iss);
        CHECK(lex.get() == TOK_VAR);
        CHECK(lex.identifier() == "A13");
        CHECK(lex.get() == TOK_EOF);
    }

    SUBCASE("Lex Expression: a Xab ab abcXYZsitrX aX XyZ")
    {
        std::istringstream iss("a Xab ab\n\t abcXYZsitrX aX XyZ");
        Lexer lex(iss);
        CHECK(lex.get() == TOK_ATOM_LOWER);
        CHECK(lex.identifier() == "a");
        CHECK(lex.get() == TOK_VAR);
        CHECK(lex.identifier() == "Xab");
        CHECK(lex.get() == TOK_ATOM_LOWER);
        CHECK(lex.identifier() == "ab");
        CHECK(lex.get() == TOK_ATOM_LOWER);
        CHECK(lex.identifier() == "abcXYZsitrX");
        CHECK(lex.get() == TOK_ATOM_LOWER);
        CHECK(lex.identifier() == "aX");
        CHECK(lex.get() == TOK_VAR);
        CHECK(lex.identifier() == "XyZ");
    }

    SUBCASE("Lex Expression: bigger(elephant, horse).")
    {
        std::istringstream iss("bigger(elephant, horse).");
        Lexer lex(iss);
        CHECK(lex.get() == TOK_ATOM_LOWER);
        CHECK(lex.identifier() == "bigger");
        CHECK(lex.get() == TOK_LPAR);
        CHECK(lex.get() == TOK_ATOM_LOWER);
        CHECK(lex.identifier() == "elephant");
        CHECK(lex.get() == TOK_COMMA);
        CHECK(lex.get() == TOK_ATOM_LOWER);
        CHECK(lex.identifier() == "horse");
        CHECK(lex.get() == TOK_RPAR);
        CHECK(lex.get() == TOK_PERIOD);
    }

    SUBCASE("Lex Expression: is_bigger(X, Y) :- bigger(X, Z), is_bigger(Z, Y).")
    {
        std::istringstream iss("is_bigger(X, Y) :- bigger(X, Z), is_bigger(Z, Y).");
        Lexer lex(iss);
        CHECK(lex.get() == TOK_ATOM_LOWER);
        CHECK(lex.identifier() == "is_bigger");
        CHECK(lex.get() == TOK_LPAR);
        CHECK(lex.get() == TOK_VAR);
        CHECK(lex.identifier() == "X");
        CHECK(lex.get() == TOK_COMMA);
        CHECK(lex.get() == TOK_VAR);
        CHECK(lex.identifier() == "Y");
        CHECK(lex.get() == TOK_RPAR);
        CHECK(lex.get() == TOK_IF);
        CHECK(lex.get() == TOK_ATOM_LOWER);
        CHECK(lex.identifier() == "bigger");
        CHECK(lex.get() == TOK_LPAR);
        CHECK(lex.get() == TOK_VAR);
        CHECK(lex.identifier() == "X");
        CHECK(lex.get() == TOK_COMMA);
        CHECK(lex.get() == TOK_VAR);
        CHECK(lex.identifier() == "Z");
        CHECK(lex.get() == TOK_RPAR);
        CHECK(lex.get() == TOK_COMMA);
        CHECK(lex.get() == TOK_ATOM_LOWER);
        CHECK(lex.identifier() == "is_bigger");
        CHECK(lex.get() == TOK_LPAR);
        CHECK(lex.get() == TOK_VAR);
        CHECK(lex.identifier() == "Z");
        CHECK(lex.get() == TOK_COMMA);
        CHECK(lex.get() == TOK_VAR);
        CHECK(lex.identifier() == "Y");
        CHECK(lex.get() == TOK_RPAR);
        CHECK(lex.get() == TOK_PERIOD);
    }

    SUBCASE("Lex Expression: a(b, c, [H|T])")
    {
        std::istringstream iss("a(b, c, [H|T])");
        Lexer lex(iss);
        CHECK(lex.get() == TOK_ATOM_LOWER);
        CHECK(lex.identifier() == "a");
        CHECK(lex.get() == TOK_LPAR);
        CHECK(lex.get() == TOK_ATOM_LOWER);
        CHECK(lex.identifier() == "b");
        CHECK(lex.get() == TOK_COMMA);
        CHECK(lex.get() == TOK_ATOM_LOWER);
        CHECK(lex.identifier() == "c");
        CHECK(lex.get() == TOK_COMMA);
        CHECK(lex.get() == TOK_LSPAR);
        CHECK(lex.get() == TOK_VAR);
        CHECK(lex.identifier() == "H");
        CHECK(lex.get() == TOK_PIPE);
        CHECK(lex.get() == TOK_VAR);
        CHECK(lex.identifier() == "T");
        CHECK(lex.get() == TOK_RSPAR);
        CHECK(lex.get() == TOK_RPAR);
    }

    SUBCASE("Lex Expression: Random whitespace")
    {
        std::istringstream iss("  \t    \n\r\t            \n");
        Lexer lex(iss);
        CHECK(lex.get() == TOK_EOF);
    }

    SUBCASE("Lex Expression: 0")
    {
        std::istringstream iss("0  ");
        Lexer lex(iss);
        CHECK(lex.get() == TOK_CONST);
        CHECK(!lex.numericValue());
    }

    SUBCASE("Lex Expression: 0:-1254 3689,56.  5687")
    {
        std::istringstream iss("0:-1254 3689,56.  5687");
        Lexer lex(iss);
        CHECK(lex.get() == TOK_CONST);
        CHECK(!lex.numericValue());
        CHECK(lex.get() == TOK_IF);
        CHECK(lex.get() == TOK_CONST);
        CHECK(lex.numericValue() == 1254);
        CHECK(lex.get() == TOK_CONST);
        CHECK(lex.numericValue() == 3689);
        CHECK(lex.get() == TOK_COMMA);
        CHECK(lex.get() == TOK_CONST);
        CHECK(lex.numericValue() == 56);
        CHECK(lex.get() == TOK_PERIOD);
        CHECK(lex.get() == TOK_CONST);
        CHECK(lex.numericValue() == 5687);
    }

    SUBCASE("Lex Expression: __id(_,_).")
    {
        std::istringstream iss("__id(_,_).");
        Lexer lex(iss);
        CHECK(lex.get() == TOK_ATOM_LOWER);
        CHECK(lex.identifier() == "__id");
        CHECK(lex.get() == TOK_LPAR);
        CHECK(lex.get() == TOK_VAR);
        CHECK(lex.identifier() == "_");
        CHECK(lex.get() == TOK_COMMA);
        CHECK(lex.get() == TOK_VAR);
        CHECK(lex.identifier() == "_");
        CHECK(lex.get() == TOK_RPAR);
        CHECK(lex.get() == TOK_PERIOD);
    }

    SUBCASE("Lex Expression: 1 + 1")
    {
        std::istringstream iss("1 + 1");
        Lexer lex(iss);
        CHECK(lex.get() == TOK_CONST);
        CHECK(lex.numericValue() == 1);
        CHECK(lex.get() == TOK_PLUS);
        CHECK(lex.get() == TOK_CONST);
        CHECK(lex.numericValue() == 1);
    }

    SUBCASE("Lex Expression: 1 <= 1 < 2")
    {
        std::istringstream iss("1 <= 1 < 2");
        Lexer lex(iss);
        CHECK(lex.get() == TOK_CONST);
        CHECK(lex.numericValue() == 1);
        CHECK(lex.get() == TOK_LESSEQ);
        CHECK(lex.get() == TOK_CONST);
        CHECK(lex.numericValue() == 1);
        CHECK(lex.get() == TOK_LESS);
        CHECK(lex.get() == TOK_CONST);
        CHECK(lex.numericValue() == 2);
    }

    SUBCASE("Lex Expression: X is 1 < 2")
    {
        std::istringstream iss("X is 1 < 2");
        Lexer lex(iss);
        CHECK(lex.get() == TOK_VAR);
        CHECK(lex.identifier() == "X");
        CHECK(lex.get() == TOK_IS);
        CHECK(lex.get() == TOK_CONST);
        CHECK(lex.numericValue() == 1);
        CHECK(lex.get() == TOK_LESS);
        CHECK(lex.get() == TOK_CONST);
        CHECK(lex.numericValue() == 2);
    }
}