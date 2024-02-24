#include "WAMCode.hpp"

void WAMCode::addInstructions(const std::vector<Instruction*> &instructions)
{
    m_Program.insert(m_Program.end(), instructions.begin(), instructions.end());
}

void WAMCode::dump(std::ostream &os)
{
    for (const auto &instruction : m_Program)
    {
        instruction->print(os);
        os << std::endl;
    }
}

void WAMCode::addLabel(const Label & label) {
    m_Labels.insert({label, m_Program.size()});
}