#include "Compiler.hpp"

#include "ast/AST.hpp"

Compiler::Compiler(std::istream &is)
    : m_Parser(Lexer(is))
{
}

void Compiler::compile(void)
{
    m_Parser.parse();
    m_Code = m_Parser.getAST()->codegen(m_CCtx);
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