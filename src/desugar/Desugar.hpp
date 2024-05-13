#pragma once

#include <string>

#include "../compiler/ast/StructNode.hpp"

class Desugar
{
public:
    static std::string toPeano(size_t num, bool underscores = false);
    static size_t fromPeano(const std::string &num, bool underscores = false);

    static std::shared_ptr<TermNode> toPeanoNode(size_t num, bool underscores = false);

    static std::string replacePeano(const std::string &str);
};