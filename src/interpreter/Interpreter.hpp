#pragma once

#include "../Renderer.hpp"
#include "WAMState.hpp"
#include "../wam_code/WAMCode.hpp"
#include "../wam_code/instruction/Instruction.hpp"

#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <string>
#include <map>

using Label = std::string;
using Result = std::pair<bool, std::map<std::string, std::string>>;

class Interpreter
{
public:
    Interpreter(const WAMCode &wamCode, const Renderer &renderer = Renderer());

    /**
     * @brief Runs the REPL environment until query in the form of "halt" is entered
     * @return True if loop should continue, false if halt query was entered
     */
    bool run(void);

    /**
     * @brief Compiles the query
     * @param query String representation of the query
     * @return Generated query bytecode
     */
    WAMCode compileQuery(const std::string &query);

    /**
     * @brief Evaluates currenty query that was set by the setQuery method
     * @return Returns a pair of bool and mapping of query variables to its values.
     */
    Result evaluateQuery(void);

    /**
     * @brief Checks whether ; is entered after query has been evaluated.
     * If it has, the machine is prompted to look for another solution
     * @param is Input stream from which to read the input after query has been evaluated
     * @return True if ; was entered and search for different solution should be attempted
     */
    bool nextAnswer(std::istream &is);

    /**
     * @brief Sets a query as a current query, merging the source code bytecode with the query bytecode,
     * and setting the PC register to the first instruction of the query.
     * @param query Query to set as a current query
     */
    void setQuery(const WAMCode &query);

    /**
     * @brief CLears the current query, removes its label and its instructions from the source code bytecode
     */
    void clearQuery(void);
    void setDumpOnly(bool dumpOnly);

private:
    std::shared_ptr<Instruction> fetch(void);
    void execute(std::shared_ptr<Instruction> instr);

    bool m_DumpOnly = false;

    WAMState m_State;
    WAMCode m_Program;
    Renderer m_Renderer;

    WAMCode m_CurrentQuery;

    const Label m_QueryLabel = "__query";
};