#include "WAMCode.hpp"

WAMCode::WAMCode(const WAMCode &wamCode)
    : m_AddressToLabel(wamCode.m_AddressToLabel),
      m_LabelToAddress(wamCode.m_LabelToAddress)
{
    for (Instruction *instr : wamCode.m_Program)
    {
        m_Program.push_back(instr->clone());
    }
}

WAMCode &WAMCode::operator=(const WAMCode &wamCode)
{
    if (this == &wamCode)
    {
        return *this;
    }

    m_AddressToLabel = wamCode.m_AddressToLabel;
    m_LabelToAddress = wamCode.m_LabelToAddress;

    for (Instruction *instr : wamCode.m_Program)
    {
        m_Program.push_back(instr->clone());
    }

    return *this;
}

WAMCode::~WAMCode(void)
{
    for (Instruction *instr : m_Program)
        delete instr;
}

void WAMCode::addInstructions(const std::vector<Instruction *> &instructions)
{
    std::vector<Instruction *> instrCpy;
    for(auto instr : instructions)
        instrCpy.push_back(instr->clone());
    m_Program.insert(m_Program.end(), instrCpy.begin(), instrCpy.end());
}

void WAMCode::deleteInstruction(size_t idx)
{
    m_Program.erase(m_Program.begin() + idx);
}

void WAMCode::popInstructions(size_t n)
{
    while (n--)
    {
        delete m_Program.back();
        m_Program.pop_back();
    }
}

void WAMCode::updateJumpInstructions(void)
{
    for (Instruction *instr : m_Program)
    {
        BranchInstruction *jump = dynamic_cast<BranchInstruction *>(instr);
        if (jump)
        {
            jump->setAddress(getLabelAddress(jump->m_Label));
        }
    }
}

void WAMCode::merge(const WAMCode &code)
{
    addInstructions(code.m_Program);
    updateJumpInstructions();
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

void WAMCode::removeLabel(const Label &label)
{
    size_t address = m_LabelToAddress[label];
    m_LabelToAddress.erase(label);
    m_AddressToLabel.erase(address);
}

size_t WAMCode::getLabelAddress(const Label &label)
{
    return m_LabelToAddress[label];
}