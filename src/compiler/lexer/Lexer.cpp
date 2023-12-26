#include "Lexer.hpp"

#include <cctype>
#include <iostream>

Lexer::Lexer ( std::istream & is )
: m_Source ( is )
{}

const std::string & Lexer::identifier ( void ) {
    return m_Identifier;
}

int Lexer::numericValue ( void ) {
    return m_NumericValue;
}

Token Lexer::lexIdentifier( void ) {
    for ( char c = m_Source . get ( ); isalnum ( c ) || c == '_'; c = m_Source . get ( ) ) {
        m_Identifier += c;
    }
        
    if ( islower(m_Identifier[0]) ) return TOK_ATOM_LOWER;
    return TOK_ATOM_VAR;
}

Token Lexer::getToken ( void ) {
    char c = m_Source . peek( );
    while ( isspace ( c ) ) {
        m_Source . get ( );
        c = m_Source . peek ( );
    }

    if ( isalpha( c ) ) {
        m_Identifier = "";
        return lexIdentifier ( );
    }

    return TOK_EOF;
}