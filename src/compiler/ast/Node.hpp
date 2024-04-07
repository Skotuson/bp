#pragma once

#include <string>

#include "../CompilationContext.hpp"

struct Node
{
    virtual ~Node(void) = default;
    virtual void codegen(CompilationContext &cctx) = 0;
    virtual void print(const std::string &indent = "") = 0;
};