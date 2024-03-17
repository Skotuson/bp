#include "Interpreter.hpp"

#include <sstream>

#include "../compiler/Compiler.hpp"

Interpreter::Interpreter(const WAMCode &wamCode, const Renderer &renderer)
    : m_Program(wamCode),
      m_Renderer(renderer)
{
}

bool Interpreter::run(void)
{
    std::cout << "?> ";
    std::string query;
    std::getline(std::cin >> std::ws, query);

    m_Renderer.clearScreen(std::cout);

    // TODO: will add as an instruction
    if (query == "halt.")
        return false;

    std::string queryLabel = "query";

    // Compile query
    std::istringstream iss(queryLabel + ":-" + query);
    Compiler queryCompiler(iss);
    queryCompiler.compile();
    WAMCode queryCode = queryCompiler.dump();

    queryCode.popInstructions(1);

    m_Program.addLabel(queryLabel);
    // Add the query instructions to the other code
    m_Program.merge(queryCode);

    m_Program.dump(std::cout);

    m_State.m_ProgramCounter = m_Program.getLabelAddress(queryLabel);

    // TODO: handle emptying arg regs after sucessfully completing a goal (multiple goals in conjuction in a query)
    // Maybe delete them from arg reg after sucessfuly unifying/.... (have to check whether possible)
    Instruction *instr;
    bool skip = true;
    while ((instr = fetch()) && !m_State.m_FailFlag)
    {
        if (!skip)
        {
            std::string com = "";
            std::getline(std::cin, com);

            if (com == "state")
                std::cout << m_State << std::endl;
            if (com == "run")
                skip = true;
        }

        m_Renderer.clearScreen(std::cout);
        m_Renderer.renderCode(std::cout, m_Program, m_State.m_ProgramCounter - 1);
        std::cout << m_State << std::endl;
        std::cout << ANSI_RETURN_CURSOR;

        execute(instr);
    }

    m_Renderer.clearScreen(std::cout);

    std::cout << m_State << std::endl;

    if (m_State.m_FailFlag)
        std::cout << ANSI_COLOR_RED << "false." << ANSI_COLOR_DEFAULT << std::endl;
    else
    {
        std::cout << ANSI_COLOR_GREEN << "true." << ANSI_COLOR_DEFAULT << std::endl;
    }

    // Remove the query code
    m_Program.popInstructions(queryCode.m_Program.size());

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
    instr->execute(m_State);
}