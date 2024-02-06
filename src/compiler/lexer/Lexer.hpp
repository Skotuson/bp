#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <iostream>

enum Token
{
    TOK_ERROR,
    TOK_EOF,
    TOK_ATOM_LOWER, // e.g. elephant, b, aBC...
    TOK_VAR,        // X, Y, XYZ, Variable...
    TOK_CONST,
    TOK_COMMA,      // ,
    TOK_PERIOD,     // .
    TOK_SEMICOLON,  // ;
    TOK_LPAR,       // (
    TOK_RPAR,       // )
    TOK_LSPAR,      // [
    TOK_RSPAR,      // ]
    TOK_UNDERSCORE, // _
    TOK_PIPE,       // |
    TOK_EQUAL,      // =
    TOK_CUT,        // !
    TOK_IF          // :-
};

class Lexer
{
public:
    Lexer(std::istream &is);

    const std::string &identifier();

    int numericValue(void);

    Token get(void);

    Token peek(void);

    void match(Token tok);

private:
    /**
     * Function used for lexing lowercase atoms and variables.
     */
    Token lexIdentifier(void);

    /**
     * Function used to lex punctuation, mathematical symbols etc.
     */
    Token lexSymbol(void);

    Token lastToken = TOK_ERROR;

    std::istream &m_Source;

    std::string m_Identifier;
    int m_NumericValue;
};

std::ostream &operator<<(std::ostream &os, const Token &tok);

#endif // LEXER_H