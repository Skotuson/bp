#ifndef AST_H
#define AST_H

#include "../SymbolTable.hpp"

#include <string>
#include <vector>

struct Node
{
    virtual ~Node(void) = default;
    virtual std::string codegen(SymbolTable &st) = 0;
    virtual void print(const std::string &indent = "") = 0;
};

struct GoalNode : public Node
{
    bool m_IsGoal = true;
    size_t m_AvailableReg = 0;
};

//-------TERM NODES-------//
struct TermNode : public GoalNode
{
    TermNode(const std::string &name);
    std::string m_Name;
};

struct StructNode : public TermNode
{
    StructNode(const std::string &name, 
        std::vector<TermNode *> args = std::vector<TermNode *>());
    std::string codegen(SymbolTable &st) override;
    void print(const std::string &indent = "") override;

    std::vector<TermNode *> m_Args;
};

struct ListNode : public TermNode
{
    ListNode(const std::vector<TermNode *> &head,
             TermNode *tail = nullptr);

    std::string codegen(SymbolTable &st) override;
    void print(const std::string &indent = "") override;

    std::vector<TermNode *> m_Head;
    TermNode *m_Tail = nullptr;
};

struct VarNode : public TermNode
{
    VarNode(const std::string &name);
    std::string codegen(SymbolTable &st) override;
    void print(const std::string &indent = "") override;
};

struct ConstNode : public TermNode
{
    std::string codegen(SymbolTable &st) override;
    void print(const std::string &indent = "") override;
    int m_Value;
};
//------------------------//

struct UnificationNode : public GoalNode
{
    UnificationNode(TermNode *x, TermNode *y);
    std::string codegen(SymbolTable &st) override;
    TermNode *m_X, *m_Y;
};

struct ClauseNode : public Node
{
    ClauseNode(const std::string &head,
               std::vector<TermNode *> args,
               std::vector<GoalNode *> body);

    std::string codegen(SymbolTable &st) override;
    void print(const std::string &indent = "") override;
    std::string m_Head;
    std::vector<TermNode *> m_Args;
    std::vector<GoalNode *> m_Body;
};

struct ProgramNode : public Node
{
    virtual std::string codegen(SymbolTable &st);
    void print(const std::string &indent = "") override;
    std::vector<ClauseNode *> m_Clauses;
};

#endif