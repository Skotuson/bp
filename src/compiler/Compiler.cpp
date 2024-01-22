#include "Compiler.hpp"

#include "ast/AST.hpp"

Compiler::Compiler ( std::istream & is, std::ostream & os )
: m_Parser ( Lexer ( is ) ),
  m_Out    ( os )
{}

void Compiler::compile ( void ) {
    m_Parser . parse ( );
    ProgramNode * astRoot = m_Parser . getAST();
    m_Out << astRoot -> codegen ( m_Table );
}