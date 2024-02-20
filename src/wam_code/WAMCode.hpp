#ifndef WAMCODE_H
#define WAMCODE_H

#include "Instruction.hpp"

#include <map>
#include <vector>
#include <string>

using Label = std::string;

struct WAMCode
{
    std::map<Label, size_t> labels;
    std::vector<Instruction> program;
};

#endif // WAMCODE_H