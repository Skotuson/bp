#pragma once

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
    TOK_IF,         // :-

    // Arithmetic Tokens
    TOK_LESS,      // <
    TOK_LESSEQ,    // <=
    TOK_GREATER,   // >
    TOK_GREATEREQ, // >=
    TOK_PLUS,      // +
    TOK_MINUS,     // -
    TOK_MUL,       // *
    TOK_DIV,       // /
    TOK_IS         // is
};

class Lexer
{
public:
    Lexer(std::istream &is);

    /**
     * @return String containing the last identifier, either a keyword, atom or a variable
     */
    const std::string &identifier();

    /**
     * @return numeric value of the last TOK_CONST lexed
     */
    int numericValue(void);

    /**
     * @brief Lexes the next token and saves it into lastToken
     * @return Lexed token
     */
    Token get(void);

    /**
     * @return Last lexed token
     */
    Token peek(void);

    /**
     * @brief Checks whether the input token matches the last lexed one. In case of a mismatch, an exception is thrown.
     * In case of a success, lexes the next token.
     */
    void match(Token tok);

private:
    /**
     * Used for lexing lowercase atoms, variables and keywords
     */
    Token lexIdentifier(void);

    /**
     * Used to lex punctuation, mathematical symbols etc.
     */
    Token lexSymbol(void);

    /**
     * Used to lex natural numbers
     */
    Token lexNumber(void);

    // Last token lexed
    Token lastToken = TOK_ERROR;

    std::istream &m_Source;

    std::string m_Identifier;
    int m_NumericValue;
};

std::ostream &operator<<(std::ostream &os, const Token &tok);