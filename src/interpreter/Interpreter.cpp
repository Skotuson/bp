#include "Interpreter.hpp"

Interpreter::Interpreter(std::istream &is)
{
    // TODO: case when only the bytecode is provided in text form
    size_t lineNumber = 0;
    std::string line;
    while (std::getline(is, line))
    {
        // std::cout << line << std::endl;
        std::istringstream iss(line);
        std::string tok;
        while (iss >> tok)
        {
            // std::cout << tok << std::endl;
            //  Token is label
            if (tok.back() == ':')
            {
                tok.pop_back();
            }
        }
        lineNumber++;
    }
}

Interpreter::Interpreter(const WAMCode &wamCode)
    : m_Program(wamCode)
{
}

bool Interpreter::run(void)
{
    std::cout << "?> ";
    std::string query;
    std::getline(std::cin >> std::ws, query);

    // TODO: will add as an instruction
    if (query == "halt.")
        return false;

    Instruction *instr;
    while ((instr = fetch()))
        execute(instr);

    return true;
}

Instruction *Interpreter::fetch(void)
{
    return m_Program.getInstruction(m_State.m_ProgramCounter++);
}

void Interpreter::execute(Instruction *instr)
{
    instr->print(std::cout);
    std::cout << std::endl;
}