#include "Lexer.hpp"

#include <cctype>

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
        //TODO: underscore keeps the identifier from the last variable
        case '_' : return TOK_VAR;
        case '|' : return TOK_PIPE;
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
    if ( lastToken != tok ) {
        std::cout << (Token) lastToken << " =\\= " << (Token) tok << std::endl;
        throw std::runtime_error ( "Matching error" );
    }
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

std::ostream & operator << ( std::ostream & os, const Token & tok ) {
    switch ( tok ) {
        case TOK_ERROR:
            return os << "ERROR";
        case TOK_EOF:
            return os << "EOF";
        case TOK_ATOM_LOWER:
            return os << "LOWER";
        case TOK_VAR:
            return os << "VAR";
        case TOK_CONST:
            return os << "CONST";
        case TOK_COMMA:
            return os << "COMMA";
        case TOK_PERIOD:
            return os << "PERIOD";
        case TOK_SEMICOLON:
            return os << "SEMICOLON";
        case TOK_LPAR:
            return os << "LPAR";
        case TOK_RPAR:
            return os << "RPAR";
        case TOK_LSPAR:
            return os << "LSPAR";
        case TOK_RSPAR:
            return os << "RSPAR";
        case TOK_UNDERSCORE:
            return os << "UNDERSCORE";
        case TOK_PIPE:
            return os << "PIPE";
        case TOK_EQUAL:
            return os << "EQUAL";
        case TOK_CUT:
            return os << "CUT";
        case TOK_IF:
            return os << "IF";
     }
    return os << "UNKNOWN TOKEN";
}