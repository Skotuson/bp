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

size_t Desugar::fromPeano(const std::string &num, bool underscores)
{
    std::string str = "";
    size_t parenCnt = 0;

    for (const auto &c : num)
    {
        if (c == '(')
        {
            parenCnt++;
        }
        if (c == ')')
        {
            parenCnt--;
            continue;
        }
        str += c;
    }

    auto split_string = [](const std::string &str, const char delim)
    {
        std::vector<std::string> r;
        std::string tmp = "";
        for (const auto &c : str)
        {
            if (c == delim)
            {
                r.push_back(tmp);
                tmp = "";
            }
            else
                tmp += c;
        }
        r.push_back(tmp);
        return r;
    };

    std::vector<std::string> split = split_string(str, '(');

    size_t peano = 0;
    bool fail = false;

    std::string successor = "s";
    if (underscores)
    {
        successor = "__" + successor;
    }

    for (const auto &c : split)
    {
        if (c == successor)
        {
            peano++;
        }
        else if (c != "0")
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