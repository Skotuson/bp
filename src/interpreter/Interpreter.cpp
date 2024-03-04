#include "Interpreter.hpp"

#include <sstream>

#include "../compiler/Compiler.hpp"

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

    std::string queryLabel = "query";

    // Compile query
    std::istringstream iss(queryLabel + ":-" + query);
    Compiler queryCompiler(iss);
    queryCompiler.compile();
    WAMCode queryCode = queryCompiler.dump();

    // Delete the first three instructions
    for (size_t i = 0; i < 3; i++)
        queryCode.deleteInstruction(0);
    // Pop the backtrack instruction
    queryCode.popInstructions(1);

    m_Program.addLabel(queryLabel);
    // Add the query instructions to the other code
    m_Program.merge(queryCode);

    m_Program.dump(std::cout);

    m_State.m_ProgramCounter = m_Program.getLabelAddress(queryLabel);

    // TODO: handle emptying arg regs after sucessfully completing a goal (multiple goals in conjuction in a query)
    // Maybe delete them from arg reg after sucessfuly unifying/.... (have to check whether possible)
    Instruction *instr;
    while ((instr = fetch()) && !m_State.m_FailFlag)
    {
        execute(instr);
    }

    // Remove the query code
    m_Program.popInstructions(queryCode.m_Program.size());
    // TODO: add same for Jump

    // Reset the WAM
    m_State = WAMState();
    return true;
}

Instruction *Interpreter::fetch(void)
{
    return m_Program.getInstruction(m_State.m_ProgramCounter++);
}

void Interpreter::execute(Instruction *instr)
{
    std::cout << "executing ";
    instr->print(std::cout);
    std::cout << std::endl;
    instr->execute(m_State);
}