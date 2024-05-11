#include "Preprocessor.hpp"

#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>

Filepath Preprocessor::linkStandard(const Filepath &filepath)
{
    std::istringstream iss(
        "__id(A, A).\n"

        "__add(0,Y,Y)."
        "__add(__s(0),Y,__s(Y))."
        "__add(__s(X),Y,__s(Z)):- __add(X,Y,Z)."

        "__mul(0,_,0)."
        "__mul(__s(A),B,C) :-"
        "__mul(A,B,D),"
        "__add(D,B,C).");

    return linkLibrary(filepath, iss);
}

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

    if (!filepath.empty())
    {
        std::ifstream ifs(filepath);
        line = "";
        while (std::getline(ifs, line))
        {
            ofs << line;
        }
        ifs.close();
    }
    ofs.close();
    return linkedFilePath;
}