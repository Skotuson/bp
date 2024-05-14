#pragma once

#include <string>

#include "../compiler/ast/StructNode.hpp"

class Desugar
{
public:
    static std::string toPeano(size_t num, bool underscores = false);
    static size_t fromPeano(const std::string &num, bool underscores = false);

    static std::shared_ptr<TermNode> toPeanoNode(size_t num, bool underscores = false);

    /**
     * @brief Replaces all occurenses of Peano numbers represented as __s(__s(...__s(0)...)) with their natural number counterpart.
     * Doesn't provide any check as the __s predicate is reserved.
     * @param str String to replace peano numbers in
     * @return String with all peano numbers replaced with their natural numbers counterpart
     */
    static std::string replacePeano(const std::string &str);
};