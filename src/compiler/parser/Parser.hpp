#ifndef PARSER_H
#define PARSER_H

#include "../lexer/Lexer.hpp"
#include "../SymbolTable.hpp"
#include "../ast/AST.hpp"


class Parser {
    public:
        Parser ( Lexer lex )
        : m_Lex ( lex )
        {}

        bool parse ( void );

        const SymbolTable & getTable ( void );
    private:
        //--------RECURSIVE DESCENT METHODS--------//
        void Start      ( void );
        void Next       ( void );
        void Predicates ( void );
        void Pred       ( void );
        void Predicate  ( void );
        void Body       ( void );
        void BodyLower  ( void );
        void BodyCont   ( void );
        void BodyTerm   ( void );
        void Term       ( void );
        void Terms      ( void );
        void TermsCont  ( void );
        void TermLower  ( void );
        //-------------CLASS VARIABLES-------------//
        Lexer         m_Lex;
        SymbolTable   m_STable;
        Node        * m_ASTRoot;
};

#endif