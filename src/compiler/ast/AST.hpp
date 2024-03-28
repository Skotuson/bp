#pragma once

#include "../CompilationContext.hpp"
#include "../../wam_code/WAMCode.hpp"

#include <string>
#include <vector>

struct Node
{
    virtual ~Node(void) = default;
    virtual std::string codegen(CompilationContext &cctx) = 0;
    virtual void print(const std::string &indent = "") = 0;
};

struct GoalNode : public Node
{
    bool m_IsGoal = true;
    bool m_IsArg = false;
    size_t m_AvailableReg = 0;
};

//-------TERM NODES-------//
struct TermNode : public GoalNode
{
protected:
    TermNode(const std::string &name);
    std::string m_Name;

public:
    enum TermType
    {
        CONST,
        VAR,
        STRUCT,
        LIST
    };
    std::string name();
    virtual TermType type() = 0;
};

struct StructNode;

struct StructNode : public TermNode
{
    StructNode(const std::string &name,
               std::vector<TermNode *> args = std::vector<TermNode *>());
    ~StructNode(void) override;
    std::string codegen(CompilationContext &cctx) override;
    TermType type() override;
    void print(const std::string &indent = "") override;

    void unifyHead(CompilationContext &cctx);
    void unifyRHS(CompilationContext &cctx);

    std::vector<TermNode *> m_Args;

    // Used when generating code for nested complex stuctures on the RHS of a goal
    // Use m_Complex size() to determine whether has any complex object
    std::map<StructNode *, size_t> m_Complex;
};

struct ListNode : public TermNode
{
    ListNode(const std::vector<TermNode *> &head,
             TermNode *tail = nullptr);
    ~ListNode(void) override;
    std::string codegen(CompilationContext &cctx) override;
    TermType type() override;
    void print(const std::string &indent = "") override;

    std::vector<TermNode *> m_Head;
    TermNode *m_Tail = nullptr;
};

struct VarNode : public TermNode
{
    VarNode(const std::string &name);
    std::string codegen(CompilationContext &cctx) override;
    TermType type() override;
    void print(const std::string &indent = "") override;
};

struct ConstNode : public TermNode
{
    ConstNode(size_t value);
    std::string codegen(CompilationContext &cctx) override;
    TermType type() override;
    void print(const std::string &indent = "") override;
    int m_Value;
};
//------------------------//

struct UnificationNode : public GoalNode
{
    UnificationNode(TermNode *x, TermNode *y);
    std::string codegen(CompilationContext &cctx) override;
    TermNode *m_X, *m_Y;
};

struct ClauseNode : public Node
{
    ClauseNode(const std::string &head,
               std::vector<TermNode *> args,
               std::vector<GoalNode *> body);

    ~ClauseNode(void) override;

    std::string codegen(CompilationContext &cctx) override;
    void print(const std::string &indent = "") override;
    std::string m_Head;
    std::vector<TermNode *> m_Args;
    std::vector<GoalNode *> m_Body;
};

struct ProgramNode : public Node
{
    ~ProgramNode(void) override;
    virtual std::string codegen(CompilationContext &cctx);
    void print(const std::string &indent = "") override;
    std::vector<ClauseNode *> m_Clauses;
};