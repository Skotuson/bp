#include "WAMCode.hpp"

void WAMCode::addInstructions(const std::vector<Instruction *> &instructions)
{
    m_Program.insert(m_Program.end(), instructions.begin(), instructions.end());
}

Instruction *WAMCode::getInstruction(size_t pc)
{
    if (pc >= m_Program.size())
        return nullptr;
    return m_Program[pc];
}

void WAMCode::dump(std::ostream &os)
{
    size_t line = 0;
    for (const auto &instruction : m_Program)
    {
        auto it = m_AddressToLabel.find(line);
        if (it != m_AddressToLabel.end())
        {
            os << it->second << ": ";
        }
        // TODO: adjust for the label length
        os << "\t";
        instruction->print(os);
        os << std::endl;
        line++;
    }
}

void WAMCode::addLabel(const Label &label)
{
    m_AddressToLabel.insert({m_Program.size(), label});
    m_LabelToAddress.insert({label, m_Program.size()});
}