#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "WAMState.hpp"
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
    WAMState m_State;
    WAMCode m_Program;
};

#endif // INTERPRETER_H