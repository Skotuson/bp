#include "Compiler.hpp"

void Compiler::compile(std::istream &is)
{
    // Parse the source code
    Parser parser = Parser(Lexer(is));
    parser.parse();
    // Traverste the AST and generate bytecode
    parser.getAST()->codegen(m_CCtx);
    // Save the generated bytecode
    m_WAMCode = m_CCtx.code();
}

WAMCode Compiler::dump(void)
{
    return m_WAMCode;
}

void Compiler::dump(std::ostream &os)
{
    m_WAMCode.dump(os);
}