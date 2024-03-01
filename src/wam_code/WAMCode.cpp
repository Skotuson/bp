#include "WAMCode.hpp"

void WAMCode::addInstructions(const std::vector<Instruction *> &instructions)
{
    m_Program.insert(m_Program.end(), instructions.begin(), instructions.end());
}

void WAMCode::popInstructions(size_t n)
{
    while (n--)
    {
        Instruction * instr = m_Program.back();
        m_Program.pop_back();
        delete instr;
    }
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

void WAMCode::removeLabel(const Label&label)
{
    size_t address = m_LabelToAddress[label];
    m_LabelToAddress.erase(label);
    m_AddressToLabel.erase(address);
}

size_t WAMCode::getLabelAddress(const Label&label)
{
    return m_LabelToAddress[label];
}