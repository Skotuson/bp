#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <istream>

enum Token {
    TOK_EOF,
    TOK_ATOM_LOWER, //e.g. elephant, b, aBC
    TOK_ATOM_VAR,
    TOK_NUMBER,
    TOK_COMMA,      // ,
    TOK_PERIOD,     // .
    TOK_SEMICOLON,  // ;
    TOK_LPAR,       // (
    TOK_RPAR,       // )
    TOK_LSPAR,      // [
    TOK_RSPAR,      // ]
    TOK_UNDERSCORE, // _
    TOK_EQUAL,      // =
    TOK_CUT,        // !
    TOK_IF          // :-
};

class Lexer {
    public:
        Lexer( std::istream & is );
       ~Lexer() = default;

        const std::string & identifier();

        int numericValue ( void );

        Token getToken ( void );

    private:

        /**
         * Function used for lexing lowercase atoms and variables.
         */
        Token lexIdentifier ( void );

        std::istream & m_Source;

        std::string m_Identifier;
        int         m_NumericValue;
};

#endif //LEXER_H