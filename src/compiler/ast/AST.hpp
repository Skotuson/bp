#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

struct Node {
    virtual ~Node ( void ) = default;
    virtual std::string codegen ( void );
};

struct ClauseNode : public Node {
    virtual std::string codegen ( void );

    std::string m_Head;
    // std::vector<> m_Args;
    // std::vector<> m_Body;
};

struct ProgramNode : public Node {
    virtual std::string codegen ( void );

    std::vector<ClauseNode *> m_Clauses;
};

#endif