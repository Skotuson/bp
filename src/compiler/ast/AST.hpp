#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

struct Node {
    virtual ~Node ( void ) = default;
    virtual std::string codegen ( void ) = 0;
    virtual void print ( const std::string & indent = "" ) = 0;
};

struct GoalNode : public Node {

};

//-------TERM NODES-------//
struct TermNode : public GoalNode {

};

struct StructNode : public TermNode {
    StructNode ( std::vector<TermNode*> args = std::vector<TermNode *> () );
    std::string codegen ( void ) override;
    std::vector<TermNode *> m_Args;
    void print ( const std::string & indent = "" ) override;
};

struct VarNode : public TermNode {
    std::string codegen ( void ) override;
    void print ( const std::string & indent = "" ) override;
};

struct ConstNode : public TermNode {
    std::string codegen ( void ) override;
    void print ( const std::string & indent = "" ) override;
    int m_Value;
};
//------------------------//

struct ClauseNode : public Node {
    ClauseNode ( std::vector<TermNode *> args, std::vector<GoalNode *> body );
    std::string codegen ( void ) override;
    void print ( const std::string & indent = "" ) override;
    std::string m_Head;
    std::vector<TermNode *> m_Args;
    std::vector<GoalNode *> m_Body;
};

struct ProgramNode : public Node {
    virtual std::string codegen ( void );
    void print ( const std::string & indent = "" ) override;
    std::vector<ClauseNode *> m_Clauses;
};

#endif