#include "Lexer.hpp"

Lexer::Lexer ( std::istream & is )
: m_Source ( is )
{}

const std::string & Lexer::identifier ( void ) {
    return m_Identifier;
}

int Lexer::numericValue ( void ) {
    return m_NumericValue;
}

Token Lexer::getToken ( void ) {

}