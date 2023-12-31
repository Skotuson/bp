#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

struct Node {
    virtual ~Node ( void ) = default;
    virtual std::string codegen ( void );
};

struct GoalNode : public Node {

};

//-------TERM NODES-------//
struct TermNode : public GoalNode {

};

struct StructNode : public TermNode {

};

struct VarNode : public TermNode {

};

struct ConstNode : public TermNode {

};
//------------------------//

struct ClauseNode : public Node {
    virtual std::string codegen ( void );
    std::string m_Head;
    std::vector<TermNode *> m_Args;
    std::vector<GoalNode *> m_Body;
};

struct ProgramNode : public Node {
    virtual std::string codegen ( void );
    std::vector<ClauseNode *> m_Clauses;
};

#endif