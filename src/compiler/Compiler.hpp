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
    void compile(std::istream &is);
    WAMCode dump(void);
    void dump(std::ostream &os);

private:
    CompilationContext m_CCtx;
    WAMCode m_WAMCode;
};