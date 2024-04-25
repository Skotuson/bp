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

    setQuery(compileQuery(query));

    if (m_State.halt())
    {
        return false;
    }

    if (m_Renderer.step())
    {
        m_Renderer.clearScreen(std::cout);
    }

    while (42)
    {
        auto [success, vars] = evaluateQuery();

        if (m_Renderer.step())
        {
            m_Renderer.clearScreen(std::cout);
        }

        std::cout << m_State << std::endl;

        if (!success)
        {
            std::cout << ANSI_COLOR_RED << "false." << ANSI_COLOR_DEFAULT << std::endl;
            break;
        }
        else
        {
            std::cout << ANSI_COLOR_GREEN << "true." << ANSI_COLOR_DEFAULT << std::endl;
            for (const auto &[var, value] : vars)
            {
                std::cout << var << " = " << value << std::endl;
            }

            std::string com = "";
            std::getline(std::cin, com);
            if (com != ";")
            {
                break;
            }
            m_State.m_EnvironmentRegister = m_State.BReg();
            std::shared_ptr<FailInstruction> fi = std::make_shared<FailInstruction>();
            fi->execute(m_State);
        }
    }

    clearQuery();

    // Reset the WAM
    m_State = WAMState();
    return true;
}

WAMCode Interpreter::compileQuery(const std::string &query)
{
    // TODO: will add as an instruction
    if (query == "halt.")
    {
        m_State.m_HaltFlag = true;
        return WAMCode();
    }

    // Compile query
    std::istringstream iss(m_QueryLabel + ":-" + query);
    Compiler queryCompiler;
    queryCompiler.compile(iss);

    WAMCode queryCode = queryCompiler.dump();
    // Pop backtrack 
    queryCode.popInstructions(1);
    return queryCode;
}

Result Interpreter::evaluateQuery(void)
{
    // TODO: handle emptying arg regs after sucessfully completing a goal (multiple goals in conjuction in a query)
    // Code as a different clause (different arity, e.g. bigger/1 and bigger/0)
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

    Result r;
    if (m_State.m_FailFlag)
    {
        r = {false, {}};
    }
    else
    {
        std::map<std::string, std::string> vars;
        for (const auto &v : m_CurrentQuery.getVariables())
        {
            std::string value = m_State.variableToString(0, v.first);
            if (v.second != value)
            {
                vars.insert({v.second, value});
            }
        }
        r = {true, vars};
    }
    return r;
}

void Interpreter::setQuery(const WAMCode &query)
{
    m_CurrentQuery = query;
    m_Program.addLabel(m_QueryLabel);
    // Add the query instructions to the other code
    m_Program.merge(query);
    m_State.m_ProgramCounter = m_Program.getLabelAddress(m_QueryLabel);
}

void Interpreter::clearQuery(void)
{
    // Remove the query code
    m_Program.popInstructions(m_CurrentQuery.size());
    m_Program.removeLabel(m_QueryLabel);
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
    // if (!m_Renderer.step())
    //{
    //     std::cout << ANSI_COLOR_B_GREEN << *instr << ANSI_COLOR_DEFAULT << std::endl;
    // }
    instr->execute(m_State);
}