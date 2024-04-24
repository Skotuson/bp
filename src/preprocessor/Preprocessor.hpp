#pragma once

#include <string>
#include <vector>

using Filepath = std::string;

class Preprocessor
{
    public:
      Filepath linkFiles(const std::vector<Filepath> & paths);
    private:
        size_t m_FilesCreated = 0;
};