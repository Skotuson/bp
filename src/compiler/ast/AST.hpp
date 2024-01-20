#ifndef AST_H
#define AST_H

#include "../SymbolTable.hpp"

#include <string>
#include <vector>

struct Node {
    virtual ~Node ( void ) = default;
    virtual std::string codegen ( SymbolTable & st ) = 0;
    virtual void print ( const std::string & indent = "" ) = 0;
};

struct GoalNode : public Node {

};

//-------TERM NODES-------//
struct TermNode : public GoalNode {

};

struct StructNode : public TermNode {
    StructNode ( const std::string      & name, 
                 std::vector<TermNode*>   args = std::vector<TermNode *> () );
    std::string codegen ( SymbolTable & st ) override;
    void print ( const std::string & indent = "" ) override;

    std::string             m_Name;
    std::vector<TermNode *> m_Args;
};

struct VarNode : public TermNode {
    VarNode ( const std::string & name );
    std::string codegen ( SymbolTable & st ) override;
    void print ( const std::string & indent = "" ) override;

    std::string m_Name;
};

struct ConstNode : public TermNode {
    std::string codegen ( SymbolTable & st ) override;
    void print ( const std::string & indent = "" ) override;
    int m_Value;
};
//------------------------//

struct ClauseNode : public Node {
    ClauseNode ( const std::string       & head, 
                 std::vector<TermNode *>   args, 
                 std::vector<GoalNode *>   body );

    std::string codegen ( SymbolTable & st ) override;
    void print ( const std::string & indent = "" ) override;
    std::string m_Head;
    std::vector<TermNode *> m_Args;
    std::vector<GoalNode *> m_Body;
};

struct ProgramNode : public Node {
    virtual std::string codegen ( SymbolTable & st );
    void print ( const std::string & indent = "" ) override;
    std::vector<ClauseNode *> m_Clauses;
};

#endif