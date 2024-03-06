#ifndef INTERPRETER_H
#define INTERPRETER_H

// TODO: pragma once

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
    // TODO: split
    Interpreter(std::istream &is);
    Interpreter(const WAMCode &wamCode);
    
    bool run(void);

private:
    Instruction * fetch(void);
    void execute(Instruction *instr);

    WAMState m_State;
    WAMCode m_Program;
};

#endif // INTERPRETER_H