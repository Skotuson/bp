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
    
    m_Source . unget();

    if ( islower(m_Identifier[0]) ) return TOK_ATOM_LOWER;
    return TOK_VAR;
}

Token Lexer::lexSymbol ( void ) {
    char c = m_Source . get ( );
    switch ( c ) {
        case ',' : return TOK_COMMA;
        case '.' : return TOK_PERIOD;
        case ';' : return TOK_SEMICOLON;
        case '(' : return TOK_LPAR;
        case ')' : return TOK_RPAR;
        case '[' : return TOK_LSPAR;
        case ']' : return TOK_RSPAR;
        case '_' : return TOK_UNDERSCORE;
        case '=' : return TOK_EQUAL;
        case ':' : 
            c = m_Source . get ( );
            switch ( c ) {
                case '-' : return TOK_IF;
                default : return TOK_ERROR;
            }
        case '!' : return TOK_CUT;
        default : return TOK_ERROR;
    }
}

void Lexer::match ( Token tok ) {
    if ( lastToken != tok )
        throw std::runtime_error("Matching error");
    get ( );
}

Token Lexer::get ( void ) {
    char c = m_Source . peek( );
    while ( isspace ( c ) ) {
        m_Source . get ( );
        c = m_Source . peek ( );
    }

    if ( isalpha( c ) ) {
        m_Identifier = "";
        return lastToken = lexIdentifier ( );
    }

    if ( c != EOF )
        return lastToken = lexSymbol ( );

    return lastToken = TOK_EOF;
}

Token Lexer::peek ( void ) {
    return lastToken;
}