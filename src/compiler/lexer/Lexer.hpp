#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <istream>

enum Token {
    TOK_EOF = -1,
    TOK_IDENTIFIER = -2,
    TOK_NUMBER = -3,
};

class Lexer {
    public:
        Lexer( std::istream & is );
       ~Lexer() = default;

        const std::string & identifier();

        int numericValue ( void );

        Token getToken ( void );

    private:
        std::istream & m_Source;

        std::string m_Identifier;
        int         m_NumericValue;
};

#endif //LEXER_H