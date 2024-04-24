#include "Compiler.hpp"

void Compiler::compile(std::istream &is)
{
    Parser parser = Parser(Lexer(is));
    parser.parse();
    parser.getAST()->codegen(m_CCtx);
    m_WAMCode = m_CCtx.code();
}

void Compiler::dump(std::ostream &os)
{
    m_WAMCode.dump(os);
}

WAMCode Compiler::dump(void)
{
    return m_WAMCode;
}