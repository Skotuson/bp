#include "CompilationContext.hpp"

void CompilationContext::add(const std::string &symbol, std::shared_ptr<TableEntry> entry)
{
    if (!m_SymbolTable.count(symbol))
        m_SymbolTable.insert({symbol, entry});
}

std::shared_ptr<TableEntry> CompilationContext::get(const std::string &symbol)
{
    auto it = m_SymbolTable.find(symbol);
    if (it != m_SymbolTable.end())
        return it->second;
    return nullptr;
}

void CompilationContext::addInstruction(std::shared_ptr<Instruction> instr)
{
    m_GeneratedCode.addInstructions({instr});
}

void CompilationContext::addInstructions(const std::vector<std::shared_ptr<Instruction>> &instructions)
{
    m_GeneratedCode.addInstructions(instructions);
}

void CompilationContext::addLabel(const Label &label)
{
    m_GeneratedCode.addLabel(label);
}

size_t CompilationContext::getLabelAddress(const Label &label)
{
    return m_GeneratedCode.m_LabelToAddress[label];
}

WAMCode CompilationContext::code()
{
    return m_GeneratedCode;
}

WAMCode &CompilationContext::getCode()
{
    return m_GeneratedCode;
}

std::string CompilationContext::generateTempVar(void)
{
    return "__T" + std::to_string(allocate());
}

void CompilationContext::addVariable(const std::string &variable)
{
    m_GeneratedCode.addVariable({getVarOffset(variable), variable});
}

size_t CompilationContext::allocate(void)
{
    return m_Variables.size();
}

void CompilationContext::noteVariable(const std::string &variable)
{
    if (m_Variables.count(variable))
        return;
    m_Variables.insert({variable, m_Variables.size()});
}

size_t CompilationContext::getVarOffset(const std::string &variable)
{
    return m_Variables[variable];
}

void CompilationContext::resetVariables(void)
{
    m_Variables.clear();
}

size_t CompilationContext::availableReg(void)
{
    return m_AvailableRegister;
}

void CompilationContext::setAvailableReg(size_t reg)
{
    m_AvailableRegister = reg;
}

std::string CompilationContext::getAvailableArithmeticVariable(void)
{
    return "__arithmetic" + std::to_string(m_AvailableArithmeticVariable);
}

void CompilationContext::resetAvailableArithmeticVariable(void)
{
    m_AvailableArithmeticVariable = 0;
}
void CompilationContext::incrementAvailableArithmeticVariable(void)
{
    m_AvailableArithmeticVariable++;
}

CodeGenerationMode CompilationContext::mode(void)
{
    return m_CGMode;
}

void CompilationContext::setHeadGenerationMode(void)
{
    m_CGMode = HEAD;
}

void CompilationContext::setBodyGenerationMode(void)
{
    m_CGMode = BODY;
}