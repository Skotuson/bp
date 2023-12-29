#include <cassert>

#include "compiler/lexer/LexerTest.hpp"
#include "compiler/parser/ParserTest.hpp"

int main ( void ) {
    LexerTest lexerTest;
    ParserTest parserTest;
    lexerTest . test();
    parserTest . test();
    return 0;
}