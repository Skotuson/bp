#pragma once

#include "TermNode.hpp"
#include "GoalNode.hpp"

/**
 * Node representing a singular clause in the source code, e.g. a fact bigger(elephant). or a rule p(x) :- y(x).
 */
struct ClauseNode : public Node
{
    ClauseNode(const std::string &head,
               std::vector<std::shared_ptr<TermNode>> args,
               std::vector<std::shared_ptr<GoalNode>> body);

    void codegen(CompilationContext &cctx) override;
    void print(const std::string &indent = "") override;

    /**
     * @return Head of the clause
    */
    std::string head(void);

private:
    std::string m_Head;
    std::vector<std::shared_ptr<TermNode>> m_Args;
    std::vector<std::shared_ptr<GoalNode>> m_Body;
};