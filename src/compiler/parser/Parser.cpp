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

void Parser::printAST ( void ) {
    m_ASTRoot -> print ( );
}

void Parser::Start ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_ATOM_LOWER:
            m_Lex . match ( TOK_ATOM_LOWER );
            m_ASTRoot -> m_Clauses . push_back ( Predicates ( ) );
            Next ( );
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
            Start ( );
            break;
        default:
            throw std::runtime_error ( "Next Parsing error" );
    }
} 

ClauseNode * Parser::Predicates ( void ) {
    switch ( m_Lex . peek ( ) ) {
        case TOK_PERIOD:
        case TOK_IF:
            return new ClauseNode ( std::vector<TermNode*> (), Predicate ( ) );
        case TOK_LPAR:
            m_Lex . match ( TOK_LPAR );
            return Pred ( );
        default:
            throw std::runtime_error ( "Predicates Parsing error" );
    }
} 

ClauseNode * Parser::Pred ( void ) {
    std::vector<TermNode *> args;
    std::vector<GoalNode *> body;
    switch ( m_Lex . peek ( ) ) {
        case TOK_ATOM_LOWER:
        case TOK_CONST:
        case TOK_VAR:
            args = Terms ( );
            m_Lex . match ( TOK_RPAR );
            body = Predicate ( );
            return new ClauseNode ( args, body );
        default:
            throw std::runtime_error ( "Pred Parsing error" );
    }
} 

std::vector<GoalNode *> Parser::Predicate ( void ) {
    std::vector<GoalNode *> body;
    switch ( m_Lex . peek ( ) ) {
        case TOK_PERIOD:
            m_Lex . match ( TOK_PERIOD );
            return std::vector<GoalNode *> ( );
        case TOK_IF:
            m_Lex . match ( TOK_IF );
            body = Body();
            m_Lex . match ( TOK_PERIOD );
            return body;
        default:
            throw std::runtime_error ( "Predicate Parsing error" );
    }
}

std::vector<GoalNode *> Parser::Body ( void ) {
    std::vector<GoalNode *> body, bodyCont;

    StructNode * compound;
    switch ( m_Lex . peek ( ) ) {
        case TOK_ATOM_LOWER:
            m_Lex . match ( TOK_ATOM_LOWER );
            compound = BodyLower();
            if ( ! BodyTerm() )
                body . push_back ( compound );
            bodyCont = BodyCont();
            body . insert ( body . end ( ), bodyCont . begin ( ), bodyCont . end ( ) );
            return body;
        case TOK_CONST:
            m_Lex . match ( TOK_CONST );
            m_Lex . match ( TOK_EQUAL );
            //TODO
            Term();
            bodyCont = BodyCont();
            body . insert ( body . end ( ), bodyCont . begin ( ), bodyCont . end ( ) );
            return body;
        case TOK_VAR:
            m_Lex . match ( TOK_VAR );
            m_Lex . match ( TOK_EQUAL );
            //TODO
            Term();
            bodyCont = BodyCont();
            body . insert ( body . end ( ), bodyCont . begin ( ), bodyCont . end ( ) );
            return body;
        default:
            throw std::runtime_error ( "Body Parsing error" );
    }
} 

StructNode * Parser::BodyLower ( void ) {
    std::vector<TermNode *> terms;
    switch ( m_Lex . peek ( ) ) {
        case TOK_EQUAL:
        case TOK_COMMA:
        case TOK_PERIOD:
            return new StructNode( );
        case TOK_LPAR:
            m_Lex . match ( TOK_LPAR );
            terms = Terms();
            m_Lex . match ( TOK_RPAR );
            return new StructNode( terms );
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
    std::vector<TermNode *> terms, termsCont;
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
    std::vector<TermNode *> terms;
    switch ( m_Lex . peek ( ) ) {
        case TOK_COMMA:
        case TOK_PERIOD:
        case TOK_RPAR:
            return new StructNode();
        case TOK_LPAR:
            m_Lex . match ( TOK_LPAR );
            terms = Terms( );
            m_Lex . match ( TOK_RPAR );
            return new StructNode(terms);
        default:
            throw std::runtime_error ( "Terms Parsing error" );
    }   
} 
