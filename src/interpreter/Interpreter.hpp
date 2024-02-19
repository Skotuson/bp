#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <vector>
#include <string>

//TODO: Will make a class later
using Instruction = std::string;

class Interpreter
{
public:
    Interpreter(std::istream & is);
    Instruction fetch(void);
    void decode(void);
    void execute(void);
private:
    std::vector<Instruction> program;
    size_t programCounter = 0;
};

#endif // INTERPRETER_H