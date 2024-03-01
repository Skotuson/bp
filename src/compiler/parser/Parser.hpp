#ifndef PARSER_H
#define PARSER_H

#include "../lexer/Lexer.hpp"
#include "../ast/AST.hpp"

class Parser
{
public:
    Parser(Lexer lex)
        : m_Lex(lex)
    {
    }

    ~Parser(void);

    bool parse(void);

    ProgramNode *getAST(void);

    void printAST(void);

private:
    //--------RECURSIVE DESCENT METHODS--------//
    void Start(void);
    void Next(void);
    ClauseNode *Predicates(void);
    ClauseNode *Pred(const std::string &head);
    std::vector<GoalNode *> Predicate(void);
    std::vector<GoalNode *> Body(void);
    StructNode *BodyLower(void);
    std::vector<GoalNode *> BodyCont(void);
    TermNode *BodyTerm(void);
    TermNode *Term(void);
    ListNode *ListInner(void);
    TermNode *ListCons(void);
    std::vector<TermNode *> Terms(void);
    std::vector<TermNode *> TermsCont(void);
    TermNode *TermLower(void);
    //-------------CLASS VARIABLES-------------//
    Lexer m_Lex;
    ProgramNode *m_ASTRoot;
};

#endif