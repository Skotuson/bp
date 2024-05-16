#include "../../doctest.h"

#include "../../../src/compiler/lexer/Lexer.hpp"
#include "../../../src/compiler/parser/Parser.hpp"
#include "../../../src/compiler/Compiler.hpp"

#include <cassert>
#include <sstream>
#include <iostream>

TEST_CASE("Parsing Error Test Suite")
{
    SUBCASE("List as a call I")
    {
        std::istringstream iss("q:-[1,2,3].");
        Lexer lex(iss);
        Parser parser(lex);
        try
        {
            parser.parse();
        }
        catch (const std::exception &e)
        {
            CHECK(std::string(e.what()) == "[] is not a callable object.");
        }
    }

    SUBCASE("Variable as a call")
    {
        std::istringstream iss("q:-X.");
        Lexer lex(iss);
        Parser parser(lex);
        try
        {
            parser.parse();
        }
        catch (const std::exception &e)
        {
            CHECK(std::string(e.what()) == "X is not a callable object.");
        }
    }
}
