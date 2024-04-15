#include "Parser.hpp"

#include "../ast/UnificationNode.hpp"
#include "../ast/ConstNode.hpp"
#include "../ast/VarNode.hpp"

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
    case TOK_ATOM_LOWER:
        m_Lex.match(TOK_ATOM_LOWER);
        m_ASTRoot->m_Clauses.push_back(Predicates());
        Next();
        break;
    default:
        throw std::runtime_error("Start Parsing error");
    }
}

void Parser::Next(void)
{
    switch (m_Lex.peek())
    {
    case TOK_EOF:
        break;
    case TOK_ATOM_LOWER:
        Start();
        break;
    default:
        throw std::runtime_error("Next Parsing error");
    }
}

std::shared_ptr<ClauseNode> Parser::Predicates(void)
{
    std::string head = m_Lex.identifier();
    switch (m_Lex.peek())
    {
    case TOK_PERIOD:
    case TOK_IF:
        return std::make_shared<ClauseNode>(head, std::vector<TermNode *>(), Predicate());
    case TOK_LPAR:
        m_Lex.match(TOK_LPAR);
        return Pred(head);
    default:
        throw std::runtime_error("Predicates Parsing error");
    }
}

std::shared_ptr<ClauseNode> Parser::Pred(const std::string &head)
{
    std::vector<TermNode *> args;
    std::vector<GoalNode *> body;

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

std::vector<GoalNode *> Parser::Predicate(void)
{
    std::vector<GoalNode *> body;
    switch (m_Lex.peek())
    {
    case TOK_PERIOD:
        m_Lex.match(TOK_PERIOD);
        return std::vector<GoalNode *>();
    case TOK_IF:
        m_Lex.match(TOK_IF);
        body = Body();
        m_Lex.match(TOK_PERIOD);
        return body;
    default:
        throw std::runtime_error("Predicate Parsing error");
    }
}

std::vector<GoalNode *> Parser::Body(void)
{
    std::vector<GoalNode *> body, bodyCont;
    StructNode *compound;
    TermNode *term;
    std::string varName = "";
    switch (m_Lex.peek())
    {
    case TOK_ATOM_LOWER:
        m_Lex.match(TOK_ATOM_LOWER);
        compound = BodyLower();
        if (!(term = BodyTerm()))
        {
            body.push_back(compound);
        }
        else
        {
            body.push_back(new UnificationNode(compound, term));
        }
        bodyCont = BodyCont();
        body.insert(body.end(), bodyCont.begin(), bodyCont.end());
        return body;
    case TOK_CONST:
        m_Lex.match(TOK_CONST);
        m_Lex.match(TOK_EQUAL);
        term = Term();
        body.push_back(new UnificationNode(new ConstNode(m_Lex.numericValue()), term));
        bodyCont = BodyCont();
        body.insert(body.end(), bodyCont.begin(), bodyCont.end());
        return body;
    case TOK_VAR:
        m_Lex.match(TOK_VAR);
        varName = m_Lex.identifier();
        m_Lex.match(TOK_EQUAL);
        term = Term();
        body.push_back(new UnificationNode(new VarNode(varName), term));
        bodyCont = BodyCont();
        body.insert(body.end(), bodyCont.begin(), bodyCont.end());
        return body;
    default:
        throw std::runtime_error("Body Parsing error");
    }
}

StructNode *Parser::BodyLower(void)
{
    std::vector<TermNode *> terms;
    std::string name = m_Lex.identifier();
    switch (m_Lex.peek())
    {
    case TOK_EQUAL:
    case TOK_COMMA:
    case TOK_PERIOD:
        return new StructNode(name);
    case TOK_LPAR:
        m_Lex.match(TOK_LPAR);
        terms = Terms();
        m_Lex.match(TOK_RPAR);
        return new StructNode(name, terms);
    default:
        throw std::runtime_error("BodyLower Parsing error");
    }
}

std::vector<GoalNode *> Parser::BodyCont(void)
{
    switch (m_Lex.peek())
    {
    case TOK_PERIOD:
        return std::vector<GoalNode *>();
    case TOK_COMMA:
        m_Lex.match(TOK_COMMA);
        return Body();
    default:
        throw std::runtime_error("BodyCont Parsing error");
    }
}

TermNode *Parser::BodyTerm(void)
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

TermNode *Parser::Term(void)
{
    std::string name = m_Lex.identifier();

    TermNode *list;
    switch (m_Lex.peek())
    {
    case TOK_ATOM_LOWER:
        m_Lex.match(TOK_ATOM_LOWER);
        return TermLower();
    case TOK_CONST:
        m_Lex.match(TOK_CONST);
        return new ConstNode(m_Lex.numericValue());
    case TOK_LSPAR:
        m_Lex.match(TOK_LSPAR);
        list = ListInner();
        m_Lex.match(TOK_RSPAR);
        return list;
    case TOK_VAR:
        m_Lex.match(TOK_VAR);
        return new VarNode(name);
    default:
        throw std::runtime_error("Term Parsing error");
    }
}

ListNode *Parser::ListInner(void)
{
    std::vector<TermNode *> list;
    TermNode *cons = nullptr;
    switch (m_Lex.peek())
    {
    case TOK_RSPAR:
        return new ListNode(list);
    case TOK_ATOM_LOWER:
    case TOK_CONST:
    case TOK_LSPAR:
    case TOK_VAR:
        list = Terms();
        // Cons is nullptr if the list is not being decomposed into [H|T]
        cons = ListCons();
        return new ListNode(list, cons);
    default:
        throw std::runtime_error("ListInner Parsing error");
    }
}

TermNode *Parser::ListCons(void)
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

std::vector<TermNode *> Parser::Terms(void)
{
    std::vector<TermNode *> terms, termsCont;
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

std::vector<TermNode *> Parser::TermsCont(void)
{
    switch (m_Lex.peek())
    {
    case TOK_RPAR:
    case TOK_PIPE:
    case TOK_RSPAR:
        return std::vector<TermNode *>(0);
    case TOK_COMMA:
        m_Lex.match(TOK_COMMA);
        return Terms();
    default:
        throw std::runtime_error("TermsCont Parsing error");
    }
}

TermNode *Parser::TermLower(void)
{
    std::vector<TermNode *> terms;
    std::string name = m_Lex.identifier();
    switch (m_Lex.peek())
    {
    case TOK_COMMA:
    case TOK_PERIOD:
    case TOK_RSPAR:
    case TOK_RPAR:
    case TOK_PIPE:
        return new StructNode(name);
    case TOK_LPAR:
        m_Lex.match(TOK_LPAR);
        terms = Terms();
        m_Lex.match(TOK_RPAR);
        return new StructNode(name, terms);
    default:
        throw std::runtime_error("Terms Parsing error");
    }
}
