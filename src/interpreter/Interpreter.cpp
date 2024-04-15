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

    if (m_Renderer.step())
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

    m_State.m_QueryVariables = queryCode.getVariables();

    queryCode.popInstructions(1);

    m_Program.addLabel(queryLabel);
    // Add the query instructions to the other code
    m_Program.merge(queryCode);

    m_Program.dump(std::cout);

    m_State.m_ProgramCounter = m_Program.getLabelAddress(queryLabel);

    // TODO: handle emptying arg regs after sucessfully completing a goal (multiple goals in conjuction in a query)
    // Code as a different clause (different arity, e.g. bigger/1 and bigger/0)
    while (42)
    {
        std::shared_ptr<Instruction> instr;
        while ((instr = fetch()) && !m_State.m_FailFlag)
        {
            if (m_Renderer.step())
            {
                m_Renderer.clearScreen(std::cout);
                m_Renderer.renderCode(std::cout, m_Program, m_State.m_ProgramCounter - 1);
                std::cout << m_State << std::endl;
                std::cout << ANSI_RETURN_CURSOR;
                std::string com = "";
                std::getline(std::cin, com);
            }

            execute(instr);
        }

        if (m_Renderer.step())
        {
            m_Renderer.clearScreen(std::cout);
        }

        std::cout << m_State << std::endl;

        if (m_State.m_FailFlag)
        {
            std::cout << ANSI_COLOR_RED << "false." << ANSI_COLOR_DEFAULT << std::endl;
            break;
        }
        else
        {
            std::cout << ANSI_COLOR_GREEN << "true." << ANSI_COLOR_DEFAULT << std::endl;
            for (const auto &v : m_State.m_QueryVariables)
            {
                std::string value = m_State.variableToString(0, v.first);
                // TODO: ugly hack probably
                if (v.second != value)
                {
                    std::cout << v.second << " = " << value << std::endl;
                }
            }

            std::string com = "";
            std::getline(std::cin, com);
            if (com != ";")
            {
                break;
            }
            m_State.m_EnvironmentRegister = m_State.getChoicePoint(m_State.BReg())->m_BCE;
            std::shared_ptr<FailInstruction> fi = std::make_shared<FailInstruction>();
            fi->execute(m_State);
        }
    }
    // Remove the query code
    m_Program.popInstructions(queryCode.m_Program.size());

    // Reset the WAM
    m_State = WAMState();
    return true;
}

std::shared_ptr<Instruction> Interpreter::fetch(void)
{
    // if (m_State.m_ProgramCounter == BAD_ADDRESS)
    //{
    //     m_State.m_FailFlag = true;
    //     return nullptr;
    // }
    return m_Program.getInstruction(m_State.m_ProgramCounter++);
}

void Interpreter::execute(std::shared_ptr<Instruction> instr)
{
    if (!m_Renderer.step())
    {
        std::cout << ANSI_COLOR_B_GREEN << *instr << ANSI_COLOR_DEFAULT << std::endl;
    }
    instr->execute(m_State);
}