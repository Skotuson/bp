#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../wam_code/WAMCode.hpp"
#include "../wam_code/Instruction.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

using Label = std::string;

class Interpreter
{
public:
    Interpreter(std::istream & is);
    Instruction fetch(void);
    Instruction decode(const std::string & str);
    void execute(const Instruction & instr);
private:
    WAMCode program;
    size_t programCounter = 0;
};

#endif // INTERPRETER_H