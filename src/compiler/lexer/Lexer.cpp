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
    for ( char c = std::cin . get ( ); isalnum ( c ) || c == '_'; c = std::cin . get ( ) )
    while ( isalnum ( c ) || c == '_' )
        m_Identifier += c;
    if ( islower(m_Identifier[0]) ) return TOK_ATOM_LOWER;
    return TOK_ATOM_VAR;
}

Token Lexer::getToken ( void ) {
    char c = m_Source . peek( );

    while ( isspace ( c ) ) 
        c = std::cin . get ( );

    if ( isalpha( c ) ) {
        m_Identifier = "";
        return lexIdentifier ( );
    }

    return TOK_EOF;
}