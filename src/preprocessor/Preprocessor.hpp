#pragma once

#include <string>
#include <vector>

using Filepath = std::string;

class Preprocessor
{
public:
  /**
   * @brief Links standard predicates to the source file
   * @param filepath Path to the source file
   * @return Returns the name of the file created by linking the source file and the standard library
   */
  Filepath linkStandard(const Filepath &filepath);

  /**
   * @brief Links input stream contents and the source file
   * @param filepath Path to the source file
   * @param is Input stream who's content to link with the source file
   * @return Returns the name of the file created by linking the source file and the contents of input stream is
   */
  Filepath linkLibrary(const Filepath &filepath, std::istream &is);

private:
  size_t m_FilesCreated = 0;
};