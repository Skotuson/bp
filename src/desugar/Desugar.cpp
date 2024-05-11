#include "Desugar.hpp"

#include "../compiler/ast/ConstNode.hpp"

#include <stdexcept>

std::string Desugar::toPeano(size_t num, bool underscores)
{
    std::string peano = "";
    for (size_t i = 0; i < num; i++)
    {
        if (underscores)
        {
            peano += "__";
        }
        peano += "s(";
    }

    peano += "0";

    for (size_t i = 0; i < num; i++)
    {
        peano += ")";
    }

    return peano;
}

size_t Desugar::fromPeano(const std::string &num)
{
    auto removeUnderscores = [](const std::string &str)
    {
        std::string r = "";
        for (const auto &c : str)
        {
            if (c != '_')
            {
                r += c;
            }
        }
        return r;
    };

    std::string n = removeUnderscores(num);

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

std::shared_ptr<TermNode> Desugar::toPeanoNode(size_t num, bool underscores)
{
    std::string successor = "s";
    if (underscores)
    {
        successor = "__" + successor;
    }

    if (!num)
    {
        return std::make_shared<ConstNode>("0");
    }

    std::shared_ptr<StructNode> peano = nullptr;
    for (size_t i = 0; i < num; i++)
    {
        std::vector<std::shared_ptr<TermNode>> args;
        if (!peano)
        {
            args = {std::make_shared<ConstNode>("0")};
            peano = std::make_shared<StructNode>("__s", args);
        }

        else
        {
            args = {peano};
            peano = std::make_shared<StructNode>("__s", args);
        }
    }
    return peano;
}