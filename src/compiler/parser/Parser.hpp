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
    std::vector<std::shared_ptr<GoalNode>> Body(void);
    std::vector<std::shared_ptr<GoalNode>> BodyCont(void);
    std::shared_ptr<TermNode> BodyTerm(void);
    std::shared_ptr<TermNode> Term(void);
    std::shared_ptr<TermNode> List(void);
    std::shared_ptr<TermNode> ListInner(void);
    std::shared_ptr<TermNode> ListCons(void);
    std::vector<std::shared_ptr<TermNode>> Terms(void);
    std::vector<std::shared_ptr<TermNode>> TermsCont(void);
    std::shared_ptr<TermNode> TermLower(void);

    std::string generateWildcardName(const std::string &varName);
    //-------------CLASS VARIABLES-------------//
    Lexer m_Lex;
    std::shared_ptr<ProgramNode> m_ASTRoot;
    size_t m_WildcardsGenerated = 0;
};