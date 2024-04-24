#pragma once

#include "../Renderer.hpp"
#include "WAMState.hpp"
#include "../wam_code/WAMCode.hpp"
#include "../wam_code/Instruction.hpp"

#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <string>
#include <map>

using Label = std::string;
using Result = std::pair<bool, std::map<size_t, std::string>>;

class Interpreter
{
public:
    Interpreter(const WAMCode &wamCode, const Renderer &renderer);

    bool run(void);
    WAMCode compileQuery(const std::string &query);
    Result evaluateQuery(WAMCode &queryCode, std::istream &is);

private:
    std::shared_ptr<Instruction> fetch(void);
    void execute(std::shared_ptr<Instruction> instr);

    WAMState m_State;
    WAMCode m_Program;
    Renderer m_Renderer;

    const Label m_QueryLabel = "__query";
};