#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

struct Node {
    virtual ~Node ( void ) = default;
    virtual std::string codegen ( void ) = 0;
    //virtual void print ( const std::string & indent = "" ) = 0;
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
};

struct VarNode : public TermNode {
    std::string codegen ( void ) override;
};

struct ConstNode : public TermNode {
    std::string codegen ( void ) override;
    int m_Value;
};
//------------------------//

struct ClauseNode : public Node {
    ClauseNode ( std::vector<TermNode *> args, std::vector<GoalNode *> body );
    std::string codegen ( void ) override;
    std::string m_Head;
    std::vector<TermNode *> m_Args;
    std::vector<GoalNode *> m_Body;
};

struct ProgramNode : public Node {
    virtual std::string codegen ( void );
    std::vector<ClauseNode *> m_Clauses;
};

#endif