#include "Interpreter.hpp"

#include <sstream>
#include <stdexcept>

#include "../desugar/Desugar.hpp"
#include "../compiler/Compiler.hpp"

Interpreter::Interpreter(const WAMCode &wamCode, const Renderer &renderer)
    : m_Program(wamCode),
      m_Renderer(renderer)
{
}

bool Interpreter::run(void)
{
    // Only dump the code and then quit
    if (m_DumpOnly)
    {
        m_Program.dump(std::cout);
        return false;
    }

    // Read the query
    std::cout << "?> ";
    std::string query;
    std::getline(std::cin >> std::ws, query);

    // Correct query if trailing period is missing
    if (query.back() != '.')
    {
        query += ".";
    }

    // Set compiled query as current query
    setQuery(compileQuery(query));

    // If a query was in the form of "halt.", the execution is ended.
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
        // Get results of query evaluation
        auto [success, vars] = evaluateQuery();

        if (m_Renderer.step())
        {
            m_Renderer.clearScreen(std::cout);
        }

        // std::cout << m_State << std::endl;

        if (!success)
        {
            std::cout << m_Renderer.printInColor("false.", ANSI_COLOR_RED) << std::endl;
            break;
        }
        else
        {
            std::cout << m_Renderer.printInColor("true.", ANSI_COLOR_GREEN) << std::endl;
            // Print the query variable values
            for (const auto &[var, value] : vars)
            {
                std::cout << var << " = " << value << std::endl;
            }

            // Look if more answers should be attempted
            if (!nextAnswer(std::cin))
            {
                break;
            }
        }
    }

    // If no more answers were prompted/can't be found, clear the query
    clearQuery();

    // Reset the WAM
    m_State = WAMState();
    return true;
}

WAMCode Interpreter::compileQuery(const std::string &query)
{
    if (query == "halt.")
    {
        m_State.setHaltFlag(true);
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
    std::shared_ptr<Instruction> instr;
    while ((instr = fetch()) && !m_State.fail())
    {
        if (m_Renderer.step())
        {
            m_Renderer.clearScreen(std::cout);
            m_Renderer.renderCode(std::cout, m_Program, m_State.PCReg() - 1);
            std::cout << m_State << std::endl;
            std::cout << ANSI_RETURN_CURSOR;
            std::string com = "";
            std::getline(std::cin, com);
        }
        execute(instr);
    }

    Result r;
    if (m_State.fail())
    {
        r = {false, {}};
    }
    else
    {
        std::map<std::string, std::string> vars;
        for (const auto &v : m_CurrentQuery.getVariables())
        {
            std::string value = m_State.variableToString(v.first);
            try
            {
                // Replace the peano number representation by its natural number counterparts
                std::string desugared = Desugar::replacePeano(value);
                value = desugared;
            }
            catch (const std::runtime_error &e)
            {
            }

            if (v.second != value)
            {
                vars.insert({v.second, value});
            }
        }
        r = {true, vars};
    }
    return r;
}

bool Interpreter::nextAnswer(std::istream &is)
{
    std::string com = "";
    std::getline(is, com);
    if (com != ";")
    {
        return false;
    }
    std::shared_ptr<Fail> fi = std::make_shared<Fail>();
    fi->execute(m_State);
    return true;
}

void Interpreter::setQuery(const WAMCode &query)
{
    m_CurrentQuery = query;
    m_State.setQueryVariables(m_CurrentQuery.getVariables());
    m_Program.addLabel(m_QueryLabel);
    // Add the query instructions to the other code
    m_Program.merge(query);
    m_State.setPCReg(m_Program.getLabelAddress(m_QueryLabel));
}

void Interpreter::clearQuery(void)
{
    // Remove the query code
    m_Program.popInstructions(m_CurrentQuery.size());
    m_Program.removeLabel(m_QueryLabel);
}

std::shared_ptr<Instruction> Interpreter::fetch(void)
{
    if (m_State.PCReg() == BAD_ADDRESS)
    {
        m_State.setFailFlag(true);
    }
    size_t pc = m_State.PCReg();
    m_State.setPCReg(m_State.PCReg() + 1);
    return m_Program.getInstruction(pc);
}

void Interpreter::execute(std::shared_ptr<Instruction> instr)
{
    // if (!m_Renderer.step())
    //{
    //     std::cout << ANSI_COLOR_B_GREEN << *instr << ANSI_COLOR_DEFAULT << std::endl;
    // }
    instr->execute(m_State);
}

void Interpreter::setDumpOnly(bool dumpOnly)
{
    m_DumpOnly = dumpOnly;
}