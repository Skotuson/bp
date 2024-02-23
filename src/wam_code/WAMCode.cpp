#include "WAMCode.hpp"

void WAMCode::addInstructions(std::vector<Instruction> & instructions) {
    program.insert(program.end(), instructions.begin(), instructions.end());
}