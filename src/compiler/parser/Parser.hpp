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

    /**
     * @brief Lexes the first token, creates an AST root and starts the parsing process using the Start() method.
     */
    bool parse(void);

    /**
     * @return Shared pointer to a AST root.
     */
    std::shared_ptr<ProgramNode> getAST(void);

    /**
     * Prints the AST
     */
    void printAST(void);

private:
    //--------HELPER METHODS--------//
    /**
     * @return Returns either a unification or is node for the lhs and rhs, depending on the operator type
     */
    std::shared_ptr<GoalNode> getGoal(Token tok, std::shared_ptr<TermNode> lhs, std::shared_ptr<TermNode> rhs);
    //--------RECURSIVE DESCENT METHODS--------//

    /**
     * @brief Used to parse clauses present in the source code
     */
    void Start(void);

    /**
     * @return Body of a rule represented as vector of GoalNodes,
     * or an empty vector in case of a fact.
     */
    std::vector<std::shared_ptr<GoalNode>> Predicate(void);

    /**
     * @return A parsed clause represented as a ClauseNode
     */
    std::shared_ptr<ClauseNode> Predicates(void);

    /**
     * @return Returns a operator of encountered token
     */
    Token Operator(void);

    /**
     * @brief Used to parse a single goal in a body of a rule.
     * @return Vector of GoalNodes
     */
    std::vector<std::shared_ptr<GoalNode>> Body(void);

    /**
     * @brief Parses a goal. If goals is not an unification or is operator, creates a call
     * and provides semantic check whether it is valid.
     * @return GoalNode representing either a call node, is node or a unification node
     */
    std::shared_ptr<GoalNode> BodyOperator(std::shared_ptr<TermNode> lhs);

    /**
     * @brief Used to parse the rest of the body and its goals
     * @return Vector of GoalNodes
     */
    std::vector<std::shared_ptr<GoalNode>> BodyCont(void);

    /**
     * @brief Used for parsing a list
     * @return A ListNode representing parsed list
     */
    std::shared_ptr<TermNode> List(void);

    /**
     * @brief Used for parsing the insides of a list
     * @return A ListNode representing parsed list
     */
    std::shared_ptr<TermNode> ListInner(void);

    /**
     * @brief Used for parsing the list decomposition, for example [H|T] or [H1,H2|T]
     * @return A ListNode representing the tail of a list (because of the decomposition), or nullptr
     * if the list is not being decomposed by the | operator.
     */
    std::shared_ptr<TermNode> ListCons(void);

    /**
     * @brief Used to parse a single term in a sequence of terms
     * @return Vector of parsed TermNodes
     */
    std::vector<std::shared_ptr<TermNode>> Terms(void);

    /**
     * @brief Used to parse remaining terms
     * @return Vector of parsed TermNodes
     */
    std::vector<std::shared_ptr<TermNode>> TermsCont(void);

    /**
     * @brief Used to parse term arguments
     * @return Returns a const node if the lower atom has no arguments, returns
     * a struct node if arguments are present.
     */
    std::shared_ptr<TermNode> TermLower(const std::string &name);

    // Methods used to parse infix expressions, mathematical expressions are represented as a struct
    // with the operator as the functor, e.g. 1 + 1 -> +(1,1)
    std::shared_ptr<TermNode> Expr2(void);
    std::shared_ptr<TermNode> Expr2R(std::shared_ptr<TermNode> lhs);
    std::shared_ptr<TermNode> Expr1(void);
    std::shared_ptr<TermNode> Expr1R(std::shared_ptr<TermNode> lhs);

    // Either returns a term (lower atom, numeric constant, list, variable) or
    // a expression in parenthesis
    std::shared_ptr<TermNode> Expr(void);

    /**
     * @return A generated variable name to be used instead of _ (mainly for printing purposes)
     */
    std::string generateWildcardName(const std::string &varName);

    //-------------CLASS VARIABLES-------------//
    Lexer m_Lex;
    std::shared_ptr<ProgramNode> m_ASTRoot;
    size_t m_WildcardsGenerated = 0;
};