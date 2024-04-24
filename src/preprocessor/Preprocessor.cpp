#include "Preprocessor.hpp"

#include <fstream>
#include <iostream>
#include <filesystem>

std::string Preprocessor::toPeano(size_t n)
{
}

size_t Preprocessor::fromPeano(const std::string &n)
{
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