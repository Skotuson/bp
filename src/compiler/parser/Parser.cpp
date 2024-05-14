#include "Parser.hpp"

#include "../ast/UnificationNode.hpp"
#include "../../desugar/Desugar.hpp"
#include "../ast/ConstNode.hpp"
#include "../ast/CallNode.hpp"
#include "../ast/VarNode.hpp"
#include "../ast/CutNode.hpp"
#include "../ast/IsNode.hpp"

bool Parser::parse(void)
{
    m_Lex.get();
    m_ASTRoot = std::make_shared<ProgramNode>();
    Start();
    return true;
}

std::shared_ptr<ProgramNode> Parser::getAST(void)
{
    return m_ASTRoot;
}

void Parser::printAST(void)
{
    m_ASTRoot->print();
}

std::shared_ptr<GoalNode> Parser::getGoal(Token tok, std::shared_ptr<TermNode> lhs, std::shared_ptr<TermNode> rhs)
{
    switch (tok)
    {
    case TOK_EQUAL:
        return std::make_shared<UnificationNode>(lhs, rhs);
    case TOK_IS:
        return std::make_shared<IsNode>(lhs, rhs);
    default:
        return nullptr;
    }
    return nullptr;
}

void Parser::Start(void)
{
    switch (m_Lex.peek())
    {
    case TOK_EOF:
        /* rule 1: Start ->  */
        break;
    case TOK_ATOM_LOWER:
        /* rule 2: Start -> lower Predicates Start */
        m_Lex.match(TOK_ATOM_LOWER);
        // Push parsed clause to ProgramNode's clauses
        m_ASTRoot->addClause(Predicates());
        // Start the process again (parse next clause)
        Start();
        break;
    default:
        throw std::runtime_error("Expected a lowercase predicate name.");
    }
}

std::vector<std::shared_ptr<GoalNode>> Parser::Predicate(void)
{
    std::vector<std::shared_ptr<GoalNode>> body;
    switch (m_Lex.peek())
    {
    case TOK_PERIOD:
        /* rule 5: Predicate -> . */
        m_Lex.match(TOK_PERIOD);
        // Clause with no body.
        return std::vector<std::shared_ptr<GoalNode>>();
    case TOK_IF:
        /* rule 6: Predicate -> :- Body . */
        m_Lex.match(TOK_IF);
        body = Body();
        m_Lex.match(TOK_PERIOD);
        return body;
    default:
        throw std::runtime_error("Period or body expected.");
    }
}

std::shared_ptr<ClauseNode> Parser::Predicates(void)
{
    std::string head = m_Lex.identifier();
    std::vector<std::shared_ptr<TermNode>> args;
    std::vector<std::shared_ptr<GoalNode>> body;
    switch (m_Lex.peek())
    {
    case TOK_PERIOD:
    case TOK_IF:
        /* rule 3: Predicates -> Predicate */
        // Clause with no head arguments
        return std::make_shared<ClauseNode>(head, std::vector<std::shared_ptr<TermNode>>(), Predicate());
    case TOK_LPAR:
        /* rule 4: Predicates -> ( Terms ) Predicate */
        m_Lex.match(TOK_LPAR);
        // Parse head arguments
        args = Terms();
        m_Lex.match(TOK_RPAR);
        // Parse clause's body
        body = Predicate();
        return std::make_shared<ClauseNode>(head, args, body);
    default:
        throw std::runtime_error("Clause argument parsing error.");
    }
}

Token Parser::Operator(void)
{
    switch (m_Lex.peek())
    {
    case TOK_EQUAL:
        /* rule 7: Operator -> = */
        m_Lex.match(TOK_EQUAL);
        return TOK_EQUAL;
    case TOK_IS:
        /* rule 8: Operator -> is */
        m_Lex.match(TOK_IS);
        return TOK_IS;
    default:
        throw std::runtime_error("Invalid operator, is or = expected.");
    }
}

std::vector<std::shared_ptr<GoalNode>> Parser::Body(void)
{
    std::vector<std::shared_ptr<GoalNode>> body, bodyCont;

    switch (m_Lex.peek())
    {
    case TOK_ATOM_LOWER:
    case TOK_CONST:
    case TOK_LSPAR:
    case TOK_VAR:
    case TOK_LPAR:
        /* rule 9: Body -> Expr2 BodyOperator BodyCont */
        {
            std::shared_ptr<TermNode> lhs = Expr2();
            body.push_back(BodyOperator(lhs));
            break;
        }
    case TOK_CUT:
        /* rule 13: Body -> ! BodyCont */
        m_Lex.match(TOK_CUT);
        body.push_back(std::make_shared<CutNode>());
        break;
    default:
        throw std::runtime_error("Invalid goal.");
    }

    // Get the rest of the body
    bodyCont = BodyCont();
    // Insert it into the body parsed so far.
    body.insert(body.end(), bodyCont.begin(), bodyCont.end());
    return body;
}

std::shared_ptr<GoalNode> Parser::BodyOperator(std::shared_ptr<TermNode> lhs)
{
    switch (m_Lex.peek())
    {
    case TOK_COMMA:
    case TOK_PERIOD:
    {
        /* rule 11: BodyOperator ->  */
        // Semantic check whether it is a valid call
        if (lhs->type() == TermNode::TermType::LIST || lhs->type() == TermNode::TermType::VAR)
        {
            std::string err = lhs->name() + " is not a callable object";
            throw std::runtime_error(err);
        }

        else if (lhs->type() == TermNode::TermType::CONST)
        {
            return std::make_shared<CallNode>(lhs->name());
        }
        else
        {
            auto snode = std::static_pointer_cast<StructNode>(lhs);
            return std::make_shared<CallNode>(snode->name(), snode->args());
        }
    }
    /* rule 12: BodyOperator -> Operator Expr2 */
    case TOK_EQUAL:
    case TOK_IS:
    {
        Token tok = Operator();
        return getGoal(tok, lhs, Expr2());
    }
    default:
        throw std::runtime_error("BodyOperator Parsing error");
    }
}

std::vector<std::shared_ptr<GoalNode>> Parser::BodyCont(void)
{
    switch (m_Lex.peek())
    {
    case TOK_PERIOD:
        /* rule 14: BodyCont ->  */
        // Body has no more goals, return empty vector
        return std::vector<std::shared_ptr<GoalNode>>();
    case TOK_COMMA:
        /* rule 15: BodyCont -> , Body */
        m_Lex.match(TOK_COMMA);
        // Parse the rest of the body
        return Body();
    default:
        throw std::runtime_error("BodyCont Parsing error");
    }
}

std::shared_ptr<TermNode> Parser::List(void)
{
    std::shared_ptr<TermNode> list;
    switch (m_Lex.peek())
    {
    case TOK_LSPAR:
        /* rule 18: List -> [ ListInner ] */
        m_Lex.match(TOK_LSPAR);
        list = ListInner();
        m_Lex.match(TOK_RSPAR);
        return list;
    default:
        throw std::runtime_error("Term Parsing error");
    }
}

std::shared_ptr<TermNode> Parser::ListInner(void)
{
    std::vector<std::shared_ptr<TermNode>> list;
    std::shared_ptr<TermNode> cons = nullptr;
    switch (m_Lex.peek())
    {
    case TOK_RSPAR:
        /* rule 19: ListInner ->  */
        return std::make_shared<ConstNode>("[]");
    case TOK_ATOM_LOWER:
    case TOK_CONST:
    case TOK_LSPAR:
    case TOK_VAR:
    case TOK_LPAR:
        /* rule 20: ListInner -> Terms ListCons */
        list = Terms();
        // Cons is nullptr if the list is not being decomposed into [H|T]
        cons = ListCons();
        return std::make_shared<ListNode>(list, cons);
    default:
        throw std::runtime_error("ListInner Parsing error");
    }
}

std::shared_ptr<TermNode> Parser::ListCons(void)
{
    switch (m_Lex.peek())
    {
    case TOK_RSPAR:
        /* rule 21: ListCons ->  */
        // List is not being decomposed, return nullptr to signal that
        return nullptr;
    case TOK_PIPE:
        /* rule 22: ListCons -> | Expr2 */
        m_Lex.match(TOK_PIPE);
        // List is being decomposed [H|T]
        return Expr2();
    default:
        throw std::runtime_error("ListCons Parsing error");
    }
}

std::vector<std::shared_ptr<TermNode>> Parser::Terms(void)
{
    std::vector<std::shared_ptr<TermNode>> terms, termsCont;
    switch (m_Lex.peek())
    {
    case TOK_ATOM_LOWER:
    case TOK_CONST:
    case TOK_LSPAR:
    case TOK_VAR:
    case TOK_LPAR:
        /* rule 23: Terms -> Expr2 TermsCont */
        terms.push_back(Expr2());
        termsCont = TermsCont();
        terms.insert(terms.end(), termsCont.begin(), termsCont.end());
        return terms;
    default:
        throw std::runtime_error("Terms Parsing error");
    }
}

std::vector<std::shared_ptr<TermNode>> Parser::TermsCont(void)
{
    switch (m_Lex.peek())
    {
    case TOK_RPAR:
    case TOK_PIPE:
    case TOK_RSPAR:
        /* rule 24: TermsCont ->  */
        return std::vector<std::shared_ptr<TermNode>>(0);
    case TOK_COMMA:
        /* rule 25: TermsCont -> , Terms */
        m_Lex.match(TOK_COMMA);
        return Terms();
    default:
        throw std::runtime_error("TermsCont Parsing error");
    }
}

std::shared_ptr<TermNode> Parser::TermLower(const std::string &name)
{
    std::vector<std::shared_ptr<TermNode>> terms;
    switch (m_Lex.peek())
    {
    case TOK_MUL:
    case TOK_DIV:
    case TOK_PLUS:
    case TOK_MINUS:
    case TOK_EQUAL:
    case TOK_IS:
    case TOK_COMMA:
    case TOK_PERIOD:
    case TOK_RSPAR:
    case TOK_RPAR:
    case TOK_PIPE:
        /* rule 26: TermLower ->  */
        return std::make_shared<ConstNode>(name);
    case TOK_LPAR:
        /* rule 27: TermLower -> ( Terms ) */
        m_Lex.match(TOK_LPAR);
        terms = Terms();
        m_Lex.match(TOK_RPAR);
        return std::make_shared<StructNode>(name, terms);
    default:
        throw std::runtime_error("Terms Parsing error");
    }
}

std::shared_ptr<TermNode> Parser::Expr2(void)
{
    switch (m_Lex.peek())
    {
    case TOK_ATOM_LOWER:
    case TOK_CONST:
    case TOK_LSPAR:
    case TOK_VAR:
    case TOK_LPAR:
        /* rule 28: Expr2 -> Expr1 Expr2R */
        return Expr2R(Expr1());
    default:
        throw std::runtime_error("Expr2 Parsing error");
    }
}

std::shared_ptr<TermNode> Parser::Expr2R(std::shared_ptr<TermNode> lhs)
{
    std::vector<std::shared_ptr<TermNode>> args = {lhs};
    switch (m_Lex.peek())
    {
    case TOK_PLUS:
        /* rule 29: Expr2R -> + Expr1 Expr2R */
        m_Lex.match(TOK_PLUS);
        args.push_back(Expr2R(Expr1()));
        return std::make_shared<StructNode>("+", args);
    case TOK_MINUS:
        /* rule 30: Expr2R -> - Expr1 Expr2R */
        m_Lex.match(TOK_MINUS);
        args.push_back(Expr2R(Expr1()));
        return std::make_shared<StructNode>("-", args);
    case TOK_EQUAL:
    case TOK_IS:
    case TOK_COMMA:
    case TOK_PERIOD:
    case TOK_RSPAR:
    case TOK_RPAR:
    case TOK_PIPE:
        /* rule 31: Expr2R ->  */
        return lhs;
    default:
        throw std::runtime_error("Expr2R Parsing error");
    }
}

std::shared_ptr<TermNode> Parser::Expr1(void)
{
    switch (m_Lex.peek())
    {
    case TOK_ATOM_LOWER:
    case TOK_CONST:
    case TOK_LSPAR:
    case TOK_VAR:
    case TOK_LPAR:
        /* rule 32: Expr1 -> Expr Expr1R */
        return Expr1R(Expr());
    default:
        throw std::runtime_error("Expr1 Parsing error");
    }
}

std::shared_ptr<TermNode> Parser::Expr1R(std::shared_ptr<TermNode> lhs)
{
    std::vector<std::shared_ptr<TermNode>> args = {lhs};
    switch (m_Lex.peek())
    {
    case TOK_MUL:
        /* rule 33: Expr1R -> * Expr Expr1R */
        m_Lex.match(TOK_MUL);
        args.push_back(Expr1R(Expr()));
        return std::make_shared<StructNode>("*", args);
    case TOK_DIV:
        /* rule 34: Expr1R -> / Expr Expr1R */
        m_Lex.match(TOK_DIV);
        args.push_back(Expr1R(Expr()));
        return std::make_shared<StructNode>("/", args);
    case TOK_PLUS:
    case TOK_MINUS:
    case TOK_EQUAL:
    case TOK_IS:
    case TOK_COMMA:
    case TOK_PERIOD:
    case TOK_RSPAR:
    case TOK_RPAR:
    case TOK_PIPE:
        /* rule 35: Expr1R ->  */
        return lhs;
    default:
        throw std::runtime_error("Expr1R Parsing error");
    }
}

std::shared_ptr<TermNode> Parser::Expr(void)
{
    std::shared_ptr<TermNode> expr;
    switch (m_Lex.peek())
    {
    case TOK_ATOM_LOWER:
    {
        /* rule 36: Expr -> lower TermLower */
        std::string identifier = m_Lex.identifier();
        m_Lex.match(TOK_ATOM_LOWER);
        return TermLower(identifier);
    }
    case TOK_CONST:
        /* rule 37: Expr -> const */
        {
            m_Lex.match(TOK_CONST);
            return Desugar::toPeanoNode(m_Lex.numericValue(), true);
        }
    case TOK_LSPAR:
        /* rule 38: Expr -> List */
        return List();
    case TOK_VAR:
    {
        /* rule 39: Expr -> var */
        std::string identifier = m_Lex.identifier();
        m_Lex.match(TOK_VAR);
        return std::make_shared<VarNode>(generateWildcardName(identifier));
    }
    case TOK_LPAR:
        /* rule 40: Expr -> ( Expr2 ) */
        m_Lex.match(TOK_LPAR);
        expr = Expr2();
        m_Lex.match(TOK_RPAR);
        return expr;
    default:
        throw std::runtime_error("Expr Parsing error");
    }
}

std::string Parser::generateWildcardName(const std::string &varName)
{
    if (varName != "_")
    {
        return varName;
    }
    return "___" + std::to_string(m_WildcardsGenerated++);
}