#pragma once

#include <string>

class Desugar
{
public:
    static std::string toPeano(size_t num, bool underscores = false);
    static size_t fromPeano(const std::string &num);
};