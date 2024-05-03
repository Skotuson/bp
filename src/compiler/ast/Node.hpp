#pragma once

#include <string>

#include "../CompilationContext.hpp"

/**
 * @brief Abstract class representing an AST node. Specifies that every node needs to implement a codegen method, i.e. a way to generate code for itself.
 */
struct Node
{
    virtual ~Node(void) = default;
    virtual void codegen(CompilationContext &cctx) = 0;
    virtual void print(const std::string &indent = "") = 0;
};