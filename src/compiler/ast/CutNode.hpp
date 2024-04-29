#pragma once

#include "GoalNode.hpp"

struct CutNode : public GoalNode
{
    void codegen(CompilationContext &cctx) override;
    void print(const std::string &indent = "") override;
};