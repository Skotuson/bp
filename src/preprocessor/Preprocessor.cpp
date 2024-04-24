#include "Preprocessor.hpp"

#include <fstream>

Filepath Preprocessor::linkFiles(const std::vector<Filepath> &paths)
{
    Filepath linkedFilePath = "__link" + std::to_string(++m_FilesCreated);
    std::ofstream ofs(linkedFilePath);

    for (const auto &path : paths)
    {
        std::ifstream ifs(path);
        std::string line;
        while(std::getline(ifs, line))
        {
            ofs << line;
        }
        ifs.close();
    }
    ofs.close();
    return linkedFilePath;
}