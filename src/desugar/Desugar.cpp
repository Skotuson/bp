#include "Desugar.hpp"

#include <stdexcept>

std::string Desugar::toPeano(size_t n)
{
    std::string peano = "";
    for (size_t i = 0; i < n; i++)
    {
        peano += "s(";
    }

    peano += "0";

    for (size_t i = 0; i < n; i++)
    {
        peano += ")";
    }

    return peano;
}

size_t Desugar::fromPeano(const std::string &n)
{
    size_t peano = 0;
    size_t parenCnt = 0;

    bool fail = false;
    for (const auto &c : n)
    {
        if (c == 's')
        {
            peano++;
        }
        else if (c == '(')
        {
            parenCnt++;
        }
        else if (c == ')')
        {
            parenCnt--;
        }
        else if (c != '0')
        {
            fail = true;
            break;
        }
    }

    if (fail || parenCnt)
    {
        throw std::runtime_error("Malformed peano number");
    }

    return peano;
}