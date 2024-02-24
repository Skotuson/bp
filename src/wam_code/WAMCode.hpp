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
    void addInstructions(const std::vector<Instruction *> &instructions);
    void dump(std::ostream &os);
    void addLabel(const Label & label);

    std::map<size_t, Label> m_Labels;
    std::vector<Instruction *> m_Program;
};

#endif // WAMCODE_H