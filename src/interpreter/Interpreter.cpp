#include "Interpreter.hpp"

Interpreter::Interpreter(std::istream &is)
{

    size_t lineNumber = 0;
    std::string line;
    while (std::getline(is, line))
    {
        std::cout << line << std::endl;
        std::istringstream iss(line);
        std::string tok;
        while (iss >> tok)
        {
            // std::cout << tok << std::endl;
            //  Token is label
            if (tok.back() == ':')
            {
                tok.pop_back();
                program.labels.insert({tok, lineNumber});
            }
        }
        lineNumber++;
    }
}

Instruction Interpreter::fetch(void)
{
    return program.program[programCounter];
}

// Instruction Interpreter::decode(const std::string & str) {
//     return Instruction();
// }

void Interpreter::execute(const Instruction &instr)
{
}