#pragma once

#include <string>
#include <vector>

using Filepath = std::string;

class Preprocessor
{
public:
  Filepath linkLibraries(const Filepath &filepath, const Filepath &libPath);

private:
  size_t m_FilesCreated = 0;
};