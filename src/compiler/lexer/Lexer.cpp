#include "Lexer.hpp"

#include <cctype>

Lexer::Lexer(std::istream &is)
    : m_Source(is)
{
}

const std::string &Lexer::identifier(void)
{
    return m_Identifier;
}

int Lexer::numericValue(void)
{
    return m_NumericValue;
}

Token Lexer::lexIdentifier(void)
{
    for (char c = m_Source.get(); isalnum(c) || c == '_'; c = m_Source.get())
    {
        m_Identifier += c;
    }

    m_Source.unget();

    if (islower(m_Identifier.front()) || (m_Identifier.front() == '_' && m_Identifier.length() > 1))
    {
        return TOK_ATOM_LOWER;
    }
    return TOK_VAR;
}

Token Lexer::lexSymbol(void)
{
    char c = m_Source.get();
    switch (c)
    {
    case ',':
        return TOK_COMMA;
    case '.':
        return TOK_PERIOD;
    case ';':
        return TOK_SEMICOLON;
    case '(':
        return TOK_LPAR;
    case ')':
        return TOK_RPAR;
    case '[':
        return TOK_LSPAR;
    case ']':
        return TOK_RSPAR;
    case '_':
        return TOK_VAR;
    case '|':
        return TOK_PIPE;
    case '=':
        return TOK_EQUAL;
    case ':':
        c = m_Source.get();
        switch (c)
        {
        case '-':
            return TOK_IF;
        default:
            return TOK_ERROR;
        }
    case '!':
        return TOK_CUT;
    case '<':
        c = m_Source.get();
        switch (c)
        {
        case '=':
            return TOK_LESSEQ;
        default:
            m_Source.unget();
            return TOK_LESS;
        }
    case '>':
        c = m_Source.get();
        switch (c)
        {
        case '=':
            return TOK_GREATEREQ;
        default:
            m_Source.unget();
            return TOK_GREATER;
        }
    case '+':
        return TOK_PLUS;
    case '-':
        return TOK_PLUS;
    case '*':
        return TOK_MUL;
    case '/':
        return TOK_DIV;
    default:
        return TOK_ERROR;
    }
}

Token Lexer::lexNumber(void)
{
    m_NumericValue = 0;
    char c = m_Source.peek();
    while (isdigit(c))
    {
        m_Source.get();
        m_NumericValue *= 10;
        m_NumericValue += c - '0';
        c = m_Source.peek();
    }
    return TOK_CONST;
}

void Lexer::match(Token tok)
{
    if (lastToken != tok)
    {
        std::cout << (Token)lastToken << " =\\= " << (Token)tok << std::endl;
        throw std::runtime_error("Matching error");
    }
    get();
}

Token Lexer::get(void)
{
    char c = m_Source.peek();
    while (isspace(c))
    {
        m_Source.get();
        c = m_Source.peek();
    }

    if (isdigit(c))
    {
        return lastToken = lexNumber();
    }

    if (isalpha(c) || c == '_')
    {
        m_Identifier = "";
        return lastToken = lexIdentifier();
    }

    if (c != EOF)
        return lastToken = lexSymbol();

    return lastToken = TOK_EOF;
}

Token Lexer::peek(void)
{
    return lastToken;
}

std::ostream &operator<<(std::ostream &os, const Token &tok)
{
    switch (tok)
    {
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
    case TOK_LESS:
        return os << "LESS";
    case TOK_LESSEQ:
        return os << "LESSEQ";
    case TOK_GREATER:
        return os << "GREATER";
    case TOK_GREATEREQ:
        return os << "GREATEREQ";
    case TOK_PLUS:
        return os << "PLUS";
    case TOK_MINUS:
        return os << "MINUS";
    case TOK_MUL:
        return os << "MUL";
    case TOK_DIV:
        return os << "DIV";
    case TOK_IS:
        return os << "IS";
    }
    return os << "UNKNOWN TOKEN";
}