#include "Parser.hpp"

bool Parser::parse ( void ) {
    m_Lex . get ( );
    Start ( );
    return true;
}

void Parser::Start ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_ATOM_LOWER:
            m_Lex . match ( TOK_ATOM_LOWER );
            Predicates();
            Next();
            break;
        default:
            throw std::runtime_error ( "Start Parsing error" );
    }
}

void Parser::Next ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_EOF:
            break;
        case TOK_ATOM_LOWER:
            Start();
            break;
        default:
            throw std::runtime_error ( "Next Parsing error" );
    }
} 

void Parser::Predicates ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_PERIOD:
        case TOK_IF:
            Predicate();
            break;
        case TOK_LPAR:
            m_Lex . match ( TOK_LPAR );
            Pred();
            break;
        default:
            throw std::runtime_error ( "Predicates Parsing error" );
    }
} 

void Parser::Pred ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_ATOM_LOWER:
        case TOK_CONST:
        case TOK_VAR:
            Terms();
            m_Lex . match ( TOK_RPAR );
            Predicate ( );
            break;
        default:
            throw std::runtime_error ( "Pred Parsing error" );
    }
} 

void Parser::Predicate ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_PERIOD:
            m_Lex . match ( TOK_PERIOD );
            break;
        case TOK_IF:
            m_Lex . match ( TOK_IF );
            Body();
            m_Lex . match ( TOK_PERIOD );
            break;
        default:
            throw std::runtime_error ( "Predicate Parsing error" );
    }
}

void Parser::Body ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_ATOM_LOWER:
            m_Lex . match ( TOK_ATOM_LOWER );
            BodyLower();
            BodyTerm();
            BodyCont();
            break;
        case TOK_CONST:
            m_Lex . match ( TOK_CONST );
            m_Lex . match ( TOK_EQUAL );
            Term();
            BodyCont();
            break;
        case TOK_VAR:
            m_Lex . match ( TOK_VAR );
            m_Lex . match ( TOK_EQUAL );
            Term();
            BodyCont();
            break;
        default:
            throw std::runtime_error ( "Body Parsing error" );
    }
} 

void Parser::BodyLower ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_EQUAL:
        case TOK_COMMA:
        case TOK_PERIOD:
            break;
        case TOK_LPAR:
            m_Lex . match ( TOK_LPAR );
            Terms();
            m_Lex . match ( TOK_RPAR );
            break;
        default:
            throw std::runtime_error ( "BodyLower Parsing error" );
    }
} 

void Parser::BodyCont ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_PERIOD:
            break;
        case TOK_COMMA:
            m_Lex . match ( TOK_COMMA );
            Body();
            break;
        default:
            throw std::runtime_error ( "BodyCont Parsing error" );
    }
} 

void Parser::BodyTerm ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_COMMA:
        case TOK_PERIOD:
            break;
        case TOK_EQUAL:
            m_Lex . match ( TOK_EQUAL );
            Term();
            break;
        default:
            throw std::runtime_error ( "BodyTerm Parsing error" );
    }
} 

void Parser::Term ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_ATOM_LOWER:
            m_Lex . match ( TOK_ATOM_LOWER );
            TermLower();
            break;
        case TOK_CONST:
            m_Lex . match ( TOK_CONST );
            break;
        case TOK_VAR:
            m_Lex . match ( TOK_VAR );
            break;
        default:
            throw std::runtime_error ( "Term Parsing error" );
    } 
} 

void Parser::Terms ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_ATOM_LOWER:
        case TOK_CONST:
        case TOK_VAR:
            m_Lex . match ( TOK_ATOM_LOWER );
            Term();
            TermsCont();
            break;
        default:
            throw std::runtime_error ( "Terms Parsing error" );
    }   
} 

void Parser::TermsCont ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_RPAR:
            break;
        case TOK_COMMA:
            m_Lex . match ( TOK_COMMA );
            Terms();
            break;
        default:
            throw std::runtime_error ( "TermsCont Parsing error" );
    }  
} 

void Parser::TermLower ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_COMMA:
        case TOK_PERIOD:
        case TOK_RPAR:
            break;
        case TOK_LPAR:
            m_Lex . match ( TOK_LPAR );
            Terms( );
            m_Lex . match ( TOK_RPAR );
            break;
        default:
            throw std::runtime_error ( "Terms Parsing error" );
    }   
} 
