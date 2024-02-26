#include "WAMCode.hpp"

void WAMCode::addInstructions(const std::vector<Instruction *> &instructions)
{
    m_Program.insert(m_Program.end(), instructions.begin(), instructions.end());
}

void WAMCode::dump(std::ostream &os)
{
    size_t line = 0;
    for (const auto &instruction : m_Program)
    {
        auto it = m_Labels.find(line);
        if (it != m_Labels.end())
        {
            os << it->second << ": ";
        }
        //TODO: adjust for the label length
        os << "\t";
        instruction->print(os);
        os << std::endl;
        line++;
    }
}

void WAMCode::addLabel(const Label &label)
{
    m_Labels.insert({m_Program.size(), label});
}