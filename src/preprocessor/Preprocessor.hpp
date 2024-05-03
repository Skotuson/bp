#pragma once

#include <string>
#include <vector>

using Filepath = std::string;

class Preprocessor
{
public:
  Filepath linkLibrary(const Filepath &filepath, std::istream &is);

private:
  size_t m_FilesCreated = 0;
};