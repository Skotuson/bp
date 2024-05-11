#pragma once

#include "../lexer/Lexer.hpp"
#include "../ast/ListNode.hpp"
#include "../ast/StructNode.hpp"
#include "../ast/ProgramNode.hpp"

class Parser
{
public:
    Parser(Lexer lex)
        : m_Lex(lex)
    {
    }

    bool parse(void);

    std::shared_ptr<ProgramNode> getAST(void);

    void printAST(void);

private:
    //--------RECURSIVE DESCENT METHODS--------//
    void Start(void);
    std::shared_ptr<ClauseNode> Predicates(void);
    std::shared_ptr<ClauseNode> Pred(const std::string &head);
    std::vector<std::shared_ptr<GoalNode>> Predicate(void);
    Token Operator(void);
    std::vector<std::shared_ptr<GoalNode>> Body(void);
    std::vector<std::shared_ptr<GoalNode>> BodyCont(void);
    std::shared_ptr<GoalNode> BodyTerm(std::shared_ptr<TermNode> lhs);
    // std::shared_ptr<TermNode> Term(void);
    std::shared_ptr<TermNode> List(void);
    std::shared_ptr<TermNode> ListInner(void);
    std::shared_ptr<TermNode> ListCons(void);
    std::vector<std::shared_ptr<TermNode>> Terms(void);
    std::vector<std::shared_ptr<TermNode>> TermsCont(void);
    std::shared_ptr<TermNode> TermLower(const std::string &name);

    std::shared_ptr<TermNode> Expr2(void);
    std::shared_ptr<TermNode> Expr2R(std::shared_ptr<TermNode> lhs);
    std::shared_ptr<TermNode> Expr1(void);
    std::shared_ptr<TermNode> Expr1R(std::shared_ptr<TermNode> lhs);
    std::shared_ptr<TermNode> Expr(void);

    std::string generateWildcardName(const std::string &varName);
    //-------------CLASS VARIABLES-------------//
    Lexer m_Lex;
    std::shared_ptr<ProgramNode> m_ASTRoot;
    size_t m_WildcardsGenerated = 0;
};