#include "Preprocessor.hpp"

#include <fstream>
#include <iostream>
#include <filesystem>

Filepath Preprocessor::linkLibrary(const Filepath &filepath, std::istream &is)
{
    Filepath linkedFilePath = "__link" + std::to_string(++m_FilesCreated);
    std::ofstream ofs(linkedFilePath);

    std::string line = "";
    while (getline(is, line))
    {
        ofs << line;
    }
    ofs << std::endl;

    std::ifstream ifs(filepath);
    line = "";
    while (std::getline(ifs, line))
    {
        ofs << line;
    }
    ifs.close();
    ofs.close();
    return linkedFilePath;
}