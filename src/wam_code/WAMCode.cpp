#include "WAMCode.hpp"

void WAMCode::addInstructions(std::vector<Instruction*> &instructions)
{
    program.insert(program.end(), instructions.begin(), instructions.end());
}

void WAMCode::dump(std::ostream &os)
{
    for (const auto &instruction : program)
    {
        instruction->print(os);
        os << std::endl;
    }
}