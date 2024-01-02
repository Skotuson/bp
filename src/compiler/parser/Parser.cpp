#include "Parser.hpp"

bool Parser::parse ( void ) {
    m_Lex . get ( );
    m_ASTRoot = new ProgramNode();
    Start ( );
    return true;
}

const SymbolTable & Parser::getTable ( void ) {
    return m_STable;
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

std::vector<GoalNode *> Parser::Body ( void ) {
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

std::vector<GoalNode *> Parser::BodyCont ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_PERIOD:
            return std::vector<GoalNode *> ();
        case TOK_COMMA:
            m_Lex . match ( TOK_COMMA );
            return Body();
        default:
            throw std::runtime_error ( "BodyCont Parsing error" );
    }
} 

TermNode * Parser::BodyTerm ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_COMMA:
        case TOK_PERIOD:
            return nullptr;
        case TOK_EQUAL:
            m_Lex . match ( TOK_EQUAL );
            return Term();
        default:
            throw std::runtime_error ( "BodyTerm Parsing error" );
    }
} 

TermNode * Parser::Term ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_ATOM_LOWER:
            m_Lex . match ( TOK_ATOM_LOWER );
            return TermLower();
        case TOK_CONST:
            m_Lex . match ( TOK_CONST );
            return new ConstNode( );
        case TOK_VAR:
            m_Lex . match ( TOK_VAR );
            return new VarNode( );
        default:
            throw std::runtime_error ( "Term Parsing error" );
    } 
} 

std::vector<TermNode *> Parser::Terms ( void ) {
    std::vector<TermNode *> terms;
    std::vector<TermNode *> termsCont;
    switch ( m_Lex . peek ( ) ) {
        case TOK_ATOM_LOWER:
        case TOK_CONST:
        case TOK_VAR:
            terms . push_back ( Term() );
            termsCont = TermsCont();
            terms . insert ( terms . end ( ), termsCont . begin ( ), termsCont . end ( ) );
            return terms;
        default:
            throw std::runtime_error ( "Terms Parsing error" );
    }   
} 

std::vector<TermNode *> Parser::TermsCont ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_RPAR:
            return std::vector<TermNode *> ( 0 );
        case TOK_COMMA:
            m_Lex . match ( TOK_COMMA );
            return Terms();
        default:
            throw std::runtime_error ( "TermsCont Parsing error" );
    }  
} 

TermNode * Parser::TermLower ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_COMMA:
        case TOK_PERIOD:
        case TOK_RPAR:
            return new StructNode();
        case TOK_LPAR:
            m_Lex . match ( TOK_LPAR );
            Terms( );
            m_Lex . match ( TOK_RPAR );
            break;
        default:
            throw std::runtime_error ( "Terms Parsing error" );
    }   
} 
