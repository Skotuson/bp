#pragma once

#include <string>

class Desugar
{
public:
    static std::string toPeano(size_t n);
    static size_t fromPeano(const std::string &n);
};