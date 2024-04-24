#pragma once

#include <string>
#include <vector>

using Filepath = std::string;

class Preprocessor
{
public:
  static std::string toPeano(size_t n);
  static size_t fromPeano(const std::string &n);

  Filepath linkLibraries(const std::string &filepath, const std::string &libPath);

private:
  size_t m_FilesCreated = 0;
};