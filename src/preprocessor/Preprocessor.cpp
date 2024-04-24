#include "Preprocessor.hpp"

#include <fstream>
#include <iostream>
#include <filesystem>

std::string Preprocessor::toPeano(size_t n)
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

size_t Preprocessor::fromPeano(const std::string &n)
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
        throw new std::runtime_error("Malformed peano number");
    }

    return peano;
}

Filepath Preprocessor::linkLibraries(const Filepath &filepath, const Filepath &libPath)
{
    Filepath linkedFilePath = "__link" + std::to_string(++m_FilesCreated);
    std::ofstream ofs(linkedFilePath);

    for (const auto &lib : std::filesystem::directory_iterator(libPath))
    {
        std::ifstream ifs(lib.path());
        std::string line;
        while (std::getline(ifs, line))
        {
            ofs << line;
        }
        ofs << std::endl;
        ifs.close();
    }

    std::ifstream ifs(filepath);
    std::string line;
    while (std::getline(ifs, line))
    {
        ofs << line;
    }
    ifs.close();
    ofs.close();
    return linkedFilePath;
}