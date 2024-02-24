#ifndef WAMCODE_H
#define WAMCODE_H

#include "Instruction.hpp"

#include <map>
#include <vector>
#include <string>
#include <ostream>

using Label = std::string;

struct WAMCode
{
    void addInstructions(std::vector<Instruction*> & instructions);
    void dump(std::ostream & os);
    std::map<Label, size_t> labels;
    std::vector<Instruction*> program;
};

#endif // WAMCODE_H