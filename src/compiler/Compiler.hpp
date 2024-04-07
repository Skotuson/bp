#pragma once

#include <iostream>
#include <string>

#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "CompilationContext.hpp"
#include "../wam_code/WAMCode.hpp"

class Compiler
{
public:
    Compiler(std::istream &is);

    void compile(void);
    WAMCode dump(void);
    void dump(std::ostream &os);

private:
    CompilationContext m_CCtx;
    Parser m_Parser = Parser(Lexer(std::cin));
    WAMCode m_WAMCode;
};