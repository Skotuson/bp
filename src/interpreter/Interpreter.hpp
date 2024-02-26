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
    Interpreter(std::istream &is);
    Interpreter(const WAMCode &wamCode);

    void run(void);

private:
    Instruction * fetch(void);
    void execute(Instruction *instr);

    WAMState m_State;
    WAMCode m_Program;
};

#endif // INTERPRETER_H