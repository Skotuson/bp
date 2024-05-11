#include "Parser.hpp"

#include "../ast/UnificationNode.hpp"
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

void Parser::Start(void)
{
    switch (m_Lex.peek())
    {
    case TOK_EOF:
        break;
    case TOK_ATOM_LOWER:
        m_Lex.match(TOK_ATOM_LOWER);
        m_ASTRoot->m_Clauses.push_back(Predicates());
        Start();
        break;
    default:
        throw std::runtime_error("Start Parsing error");
    }
}

std::shared_ptr<ClauseNode> Parser::Predicates(void)
{
    std::string head = m_Lex.identifier();
    switch (m_Lex.peek())
    {
    case TOK_PERIOD:
    case TOK_IF:
        return std::make_shared<ClauseNode>(head, std::vector<std::shared_ptr<TermNode>>(), Predicate());
    case TOK_LPAR:
        m_Lex.match(TOK_LPAR);
        return Pred(head);
    default:
        throw std::runtime_error("Predicates Parsing error");
    }
}

std::shared_ptr<ClauseNode> Parser::Pred(const std::string &head)
{
    std::vector<std::shared_ptr<TermNode>> args;
    std::vector<std::shared_ptr<GoalNode>> body;

    switch (m_Lex.peek())
    {
    case TOK_ATOM_LOWER:
    case TOK_CONST:
    case TOK_LSPAR:
    case TOK_VAR:
    case TOK_LPAR:
        args = Terms();
        m_Lex.match(TOK_RPAR);
        body = Predicate();
        return std::make_shared<ClauseNode>(head, args, body);
    default:
        throw std::runtime_error("Pred Parsing error");
    }
}

std::vector<std::shared_ptr<GoalNode>> Parser::Predicate(void)
{
    std::vector<std::shared_ptr<GoalNode>> body;
    switch (m_Lex.peek())
    {
    case TOK_PERIOD:
        m_Lex.match(TOK_PERIOD);
        return std::vector<std::shared_ptr<GoalNode>>();
    case TOK_IF:
        m_Lex.match(TOK_IF);
        body = Body();
        m_Lex.match(TOK_PERIOD);
        return body;
    default:
        throw std::runtime_error("Predicate Parsing error");
    }
}

Token Parser::Operator(void)
{
    switch (m_Lex.peek())
    {
    case TOK_EQUAL:
        m_Lex.match(TOK_EQUAL);
        return TOK_EQUAL;
    case TOK_IS:
        m_Lex.match(TOK_IS);
        return TOK_IS;
    default:
        throw std::runtime_error("Operator Parsing error");
    }
}

std::vector<std::shared_ptr<GoalNode>> Parser::Body(void)
{
    std::vector<std::shared_ptr<GoalNode>> body, bodyCont;
    std::shared_ptr<TermNode> compound;
    std::shared_ptr<GoalNode> expr;
    std::shared_ptr<TermNode> list;
    std::string varName = "";
    std::string constant = "";
    Token tok;

    auto getGoal = [](Token tok, std::shared_ptr<TermNode> lhs, std::shared_ptr<TermNode> rhs) -> std::shared_ptr<GoalNode>
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
    };

    switch (m_Lex.peek())
    {
    case TOK_ATOM_LOWER:
        constant = m_Lex.identifier();
        m_Lex.match(TOK_ATOM_LOWER);
        compound = TermLower(constant);
        // Not a unification, so it is a call
        if (!(expr = BodyTerm(compound)))
        {
            if (compound->type() == TermNode::TermType::CONST)
            {
                body.push_back(std::make_shared<CallNode>(compound->name()));
            }
            else
            {
                auto snode = std::static_pointer_cast<StructNode>(compound);
                body.push_back(std::make_shared<CallNode>(snode->name(), snode->args()));
            }
        }
        else
        {
            body.push_back(expr);
        }
        bodyCont = BodyCont();
        body.insert(body.end(), bodyCont.begin(), bodyCont.end());
        return body;
    case TOK_CONST:
        m_Lex.match(TOK_CONST);
        constant = std::to_string(m_Lex.numericValue());
        tok = Operator();
        body.push_back(getGoal(tok, std::make_shared<ConstNode>(constant), Expr2()));
        bodyCont = BodyCont();
        body.insert(body.end(), bodyCont.begin(), bodyCont.end());
        return body;
    case TOK_VAR:
        varName = generateWildcardName(m_Lex.identifier());
        m_Lex.match(TOK_VAR);
        tok = Operator();
        body.push_back(getGoal(tok, std::make_shared<VarNode>(varName), Expr2()));
        bodyCont = BodyCont();
        body.insert(body.end(), bodyCont.begin(), bodyCont.end());
        return body;
    case TOK_LSPAR:
        list = List();
        tok = Operator();
        body.push_back(getGoal(tok, list, Expr2()));
        bodyCont = BodyCont();
        body.insert(body.end(), bodyCont.begin(), bodyCont.end());
        return body;
    case TOK_CUT:
        m_Lex.match(TOK_CUT);
        body.push_back(std::make_shared<CutNode>());
        bodyCont = BodyCont();
        body.insert(body.end(), bodyCont.begin(), bodyCont.end());
        return body;
    default:
        throw std::runtime_error("Body Parsing error");
    }
}

std::vector<std::shared_ptr<GoalNode>> Parser::BodyCont(void)
{
    switch (m_Lex.peek())
    {
    case TOK_PERIOD:
        return std::vector<std::shared_ptr<GoalNode>>();
    case TOK_COMMA:
        m_Lex.match(TOK_COMMA);
        return Body();
    default:
        throw std::runtime_error("BodyCont Parsing error");
    }
}

std::shared_ptr<GoalNode> Parser::BodyTerm(std::shared_ptr<TermNode> lhs)
{
    Token tok;
    switch (m_Lex.peek())
    {
    case TOK_COMMA:
    case TOK_PERIOD:
        return nullptr;
    case TOK_EQUAL:
    case TOK_IS:
        tok = Operator();
        if (tok == TOK_EQUAL)
        {
            return std::make_shared<UnificationNode>(lhs, Expr2());
        }
        else if (tok == TOK_IS)
        {
            return std::make_shared<IsNode>(lhs, Expr2());
        }
    default:
        throw std::runtime_error("BodyTerm Parsing error");
    }
}

std::shared_ptr<TermNode> Parser::List(void)
{
    std::shared_ptr<TermNode> list;
    switch (m_Lex.peek())
    {
    case TOK_LSPAR:
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
        return std::make_shared<ConstNode>("[]");
    case TOK_ATOM_LOWER:
    case TOK_CONST:
    case TOK_LSPAR:
    case TOK_VAR:
    case TOK_LPAR:
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
        return nullptr;
    case TOK_PIPE:
        m_Lex.match(TOK_PIPE);
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
        terms.push_back(Expr());
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
        return std::vector<std::shared_ptr<TermNode>>(0);
    case TOK_COMMA:
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
    case TOK_EQUAL:
    case TOK_IS:
    case TOK_COMMA:
    case TOK_PERIOD:
    case TOK_MUL:
    case TOK_DIV:
    case TOK_PLUS:
    case TOK_MINUS:
    case TOK_RSPAR:
    case TOK_RPAR:
    case TOK_PIPE:
        return std::make_shared<ConstNode>(name);
    case TOK_LPAR:
        m_Lex.match(TOK_LPAR);
        terms = Terms();
        m_Lex.match(TOK_RPAR);
        return std::make_shared<StructNode>(name, terms);
    default:
        throw std::runtime_error("Terms Parsing error");
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

std::shared_ptr<TermNode> Parser::Expr2(void)
{
    switch (m_Lex.peek())
    {
    case TOK_ATOM_LOWER:
    case TOK_CONST:
    case TOK_LSPAR:
    case TOK_VAR:
    case TOK_LPAR:
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
        m_Lex.match(TOK_PLUS);
        args.push_back(Expr2R(Expr1()));
        return std::make_shared<StructNode>("+", args);
    case TOK_MINUS:
        m_Lex.match(TOK_MINUS);
        args.push_back(Expr2R(Expr1()));
        return std::make_shared<StructNode>("-", args);
    case TOK_COMMA:
    case TOK_PERIOD:
    case TOK_RSPAR:
    case TOK_RPAR:
    case TOK_PIPE:
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
        m_Lex.match(TOK_MUL);
        args.push_back(Expr1R(Expr()));
        return std::make_shared<StructNode>("*", args);
    case TOK_DIV:
        m_Lex.match(TOK_DIV);
        args.push_back(Expr1R(Expr()));
        return std::make_shared<StructNode>("/", args);
    case TOK_PLUS:
    case TOK_MINUS:
    case TOK_COMMA:
    case TOK_PERIOD:
    case TOK_RSPAR:
    case TOK_RPAR:
    case TOK_PIPE:
        return lhs;
    default:
        throw std::runtime_error("Expr1R Parsing error");
    }
}

std::shared_ptr<TermNode> Parser::Expr(void)
{
    std::string number;
    std::shared_ptr<TermNode> expr;
    switch (m_Lex.peek())
    {
    case TOK_ATOM_LOWER:
        number = m_Lex.identifier();
        m_Lex.match(TOK_ATOM_LOWER);
        return TermLower(number);
    case TOK_CONST:
        m_Lex.match(TOK_CONST);
        return std::make_shared<ConstNode>(std::to_string(m_Lex.numericValue()));
    case TOK_LSPAR:
        return List();
    case TOK_VAR:
        m_Lex.match(TOK_VAR);
        return std::make_shared<VarNode>(generateWildcardName(m_Lex.identifier()));
    case TOK_LPAR:
        m_Lex.match(TOK_LPAR);
        expr = Expr2();
        m_Lex.match(TOK_RPAR);
        return expr;
    default:
        throw std::runtime_error("Expr Parsing error");
    }
}
