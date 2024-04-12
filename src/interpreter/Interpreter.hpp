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

class Interpreter
{
public:
    Interpreter(const WAMCode &wamCode, const Renderer &renderer);

    bool run(void);

private:
    std::shared_ptr<Instruction> fetch(void);
    void execute(std::shared_ptr<Instruction> instr);

    WAMState m_State;
    WAMCode m_Program;
    Renderer m_Renderer;
};