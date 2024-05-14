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
    /**
     * @brief Compiles the source code found in the input stream
     * @param is Input stream to the source code
     */
    void compile(std::istream &is);

    /**
     * @return Returns bytecode represented by the WAMCode class
     */
    WAMCode dump(void);

    /**
     * @brief Prints the bytecode
     * @param os Output stream to print the bytecode to
     */
    void dump(std::ostream &os);

private:
    CompilationContext m_CCtx;
    WAMCode m_WAMCode;
};