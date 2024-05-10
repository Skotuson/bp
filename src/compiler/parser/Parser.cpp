#include "Parser.hpp"

#include "../ast/UnificationNode.hpp"
#include "../ast/ConstNode.hpp"
#include "../ast/CallNode.hpp"
#include "../ast/VarNode.hpp"
#include "../ast/CutNode.hpp"

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

std::vector<std::shared_ptr<GoalNode>> Parser::Body(void)
{
    std::vector<std::shared_ptr<GoalNode>> body, bodyCont;
    std::shared_ptr<TermNode> compound;
    std::shared_ptr<TermNode> term;
    std::shared_ptr<TermNode> list;
    std::string varName = "";
    switch (m_Lex.peek())
    {
    case TOK_ATOM_LOWER:
        m_Lex.match(TOK_ATOM_LOWER);
        compound = TermLower();
        // Not a unification, so it is a call
        if (!(term = BodyTerm()))
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
            body.push_back(std::make_shared<UnificationNode>(compound, term));
        }
        bodyCont = BodyCont();
        body.insert(body.end(), bodyCont.begin(), bodyCont.end());
        return body;
    case TOK_CONST:
        m_Lex.match(TOK_CONST);
        m_Lex.match(TOK_EQUAL);
        term = Term();
        body.push_back(std::make_shared<UnificationNode>(std::make_shared<ConstNode>(std::to_string(m_Lex.numericValue())), term));
        bodyCont = BodyCont();
        body.insert(body.end(), bodyCont.begin(), bodyCont.end());
        return body;
    case TOK_VAR:
        m_Lex.match(TOK_VAR);
        varName = generateWildcardName(m_Lex.identifier());
        m_Lex.match(TOK_EQUAL);
        term = Term();
        body.push_back(std::make_shared<UnificationNode>(std::make_shared<VarNode>(varName), term));
        bodyCont = BodyCont();
        body.insert(body.end(), bodyCont.begin(), bodyCont.end());
        return body;
    case TOK_LSPAR:
        list = List();
        m_Lex.match(TOK_EQUAL);
        term = Term();
        body.push_back(std::make_shared<UnificationNode>(list, term));
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

std::shared_ptr<TermNode> Parser::BodyTerm(void)
{
    switch (m_Lex.peek())
    {
    case TOK_COMMA:
    case TOK_PERIOD:
        return nullptr;
    case TOK_EQUAL:
        m_Lex.match(TOK_EQUAL);
        return Term();
    default:
        throw std::runtime_error("BodyTerm Parsing error");
    }
}

std::shared_ptr<TermNode> Parser::Term(void)
{
    switch (m_Lex.peek())
    {
    case TOK_ATOM_LOWER:
        m_Lex.match(TOK_ATOM_LOWER);
        return TermLower();
    case TOK_CONST:
        m_Lex.match(TOK_CONST);
        return std::make_shared<ConstNode>(std::to_string(m_Lex.numericValue()));
    case TOK_LSPAR:
        return List();
    case TOK_VAR:
        m_Lex.match(TOK_VAR);
        return std::make_shared<VarNode>(generateWildcardName(m_Lex.identifier()));
    default:
        throw std::runtime_error("Term Parsing error");
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
        return Term();
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
        terms.push_back(Term());
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

std::shared_ptr<TermNode> Parser::TermLower(void)
{
    std::vector<std::shared_ptr<TermNode>> terms;
    std::string name = m_Lex.identifier();
    switch (m_Lex.peek())
    {
    case TOK_EQUAL:
    case TOK_COMMA:
    case TOK_PERIOD:
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

void Expr3(void)
{
}

void Expr3R(void)
{
}

void Expr2(void)
{
}

void Expr2R(void)
{
}

void Expr1(void)
{
}

void Expr1R(void)
{
}

void Expr(void)
{
}
