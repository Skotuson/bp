#include "WAMState.hpp"

#include "../Renderer.hpp"

#include <limits>

void WAMState::fillRegister(std::shared_ptr<Word> word, size_t reg)
{
    m_ArgumentRegisters.fillRegister(word, reg);
}

size_t WAMState::SReg(void) const
{
    return m_Stack.size();
}

size_t WAMState::EReg(void) const
{
    return m_EnvironmentRegister;
}

size_t WAMState::BReg(void) const
{
    return m_BacktrackRegister;
}

size_t WAMState::TRReg(void) const
{
    return m_Trail.size();
}

size_t WAMState::PDLReg(void) const
{
    return m_PushDownList.size();
}

size_t WAMState::SPReg(void) const
{
    return m_StructurePointer;
}

size_t WAMState::HReg(void) const
{
    return m_Heap.size();
}

size_t WAMState::CPReg(void) const
{
    return m_ContinuationPointer;
}

size_t WAMState::PCReg(void) const
{
    return m_ProgramCounter;
}

void WAMState::setEReg(size_t ereg)
{
    m_EnvironmentRegister = ereg;
}
void WAMState::setBReg(size_t breg)
{
    m_BacktrackRegister = breg;
}
void WAMState::setSPReg(size_t spreg)
{
    m_StructurePointer = spreg;
}
void WAMState::setCPReg(size_t cpreg)
{
    m_ContinuationPointer = cpreg;
}
void WAMState::setPCReg(size_t pcreg)
{
    m_ProgramCounter = pcreg;
}

void WAMState::setWriteMode(void)
{
    m_ReadMode = false;
}

void WAMState::setReadMode(void)
{
    m_ReadMode = true;
}

bool WAMState::readMode(void) const
{
    return m_ReadMode;
}

bool WAMState::fail(void) const
{
    return m_FailFlag;
}

void WAMState::setFailFlag(bool failFlag)
{
    m_FailFlag = failFlag;
}

bool WAMState::halt(void) const
{
    return m_HaltFlag;
}

void WAMState::setHaltFlag(bool haltFlag)
{
    m_HaltFlag = haltFlag;
}

void WAMState::heapPush(std::shared_ptr<Word> word)
{
    m_Heap.push_back(word);
}

void WAMState::heapPop(void)
{
    m_Heap.pop_back();
}

std::shared_ptr<Word> WAMState::heapTop(void)
{
    return m_Heap.back();
}

std::shared_ptr<Word> WAMState::heapAt(size_t offset)
{
    return m_Heap[offset];
}

const std::vector<std::shared_ptr<Word>> &WAMState::heapRef(void)
{
    return m_Heap;
}

void WAMState::stackPush(std::shared_ptr<ChoicePoint> cp)
{
    m_Stack.push_back(cp);
}

void WAMState::stackPop(void)
{
    if (!m_Stack.empty())
    {
        m_Stack.pop_back();
    }
}

bool WAMState::stackEmpty(void)
{
    return m_Stack.empty();
}

std::shared_ptr<ChoicePoint> WAMState::stackTop(void)
{
    if (m_Stack.size())
    {
        return m_Stack.back();
    }

    return nullptr;
}

std::shared_ptr<ChoicePoint> WAMState::stackAt(size_t address)
{
    if (m_Stack.size() && address < m_Stack.size())
    {
        return m_Stack[address];
    }

    return nullptr;
}

void WAMState::trailPush(std::shared_ptr<VariableWord> word)
{
    m_Trail.push_back(word);
}

void WAMState::trailPop(void)
{
    m_Trail.pop_back();
}

std::shared_ptr<VariableWord> WAMState::trailTop(void)
{
    return m_Trail[TRReg() - 1];
}

std::shared_ptr<VariableWord> WAMState::trailAt(size_t address)
{
    return m_Trail[address];
}

void WAMState::pdlPush(const PDLTriple &pdlTriple)
{
    m_PushDownList.push_back(pdlTriple);
}

void WAMState::pdlPop(void)
{
    m_PushDownList.pop_back();
}

bool WAMState::pdlEmpty(void)
{
    return m_PushDownList.empty();
}

PDLTriple WAMState::pdlTop(void)
{
    return m_PushDownList[PDLReg() - 1];
}

std::string WAMState::variableToString(size_t offset, size_t choicePoint)
{
    auto cp = stackAt(choicePoint);
    m_QueryWords[offset]->setVisited(true);
    std::string result = cp->localVariables()[offset]->toString();

    for (size_t i = 0; i < TRReg(); i++)
    {
        trailAt(i)->setVisited(false);
    }

    return result;
}

size_t WAMState::getAllocatedVariables(void)
{
    return m_AllocatedVariables;
}
void WAMState::setAllocatedVariables(size_t allocatedVariables)
{
    m_AllocatedVariables = allocatedVariables;
}

const ArgumentRegisters &WAMState::getArgumentRegisters(void)
{
    return m_ArgumentRegisters;
}

void WAMState::setArgumentRegisters(const ArgumentRegisters &argumentRegisters)
{
    m_ArgumentRegisters = argumentRegisters;
}

std::shared_ptr<Word> WAMState::dereferenceArgumentRegister(size_t argReg)
{
    return m_ArgumentRegisters.dereferenceRegister(argReg);
}

void WAMState::setQueryVariables(const std::map<size_t, std::string> &queryVariables)
{
    m_QueryVariables = queryVariables;
}

std::string WAMState::getQueryVariableName(size_t offset)
{
    return m_QueryVariables[offset];
}

bool WAMState::isQueryVariable(size_t offset)
{
    return m_QueryVariables.count(offset);
}

void WAMState::addQueryWord(size_t offset, std::shared_ptr<VariableWord> queryWord)
{
    m_QueryWords.insert({offset, queryWord});
}

std::ostream &operator<<(std::ostream &os, const WAMState &state)
{
    auto format = [](size_t n)
    {
        return n == UNSET_REG ? "xxx" : std::to_string(n);
    };

    os << ANSI_COLOR_B_MAGENTA << "Mode:" << (state.readMode() ? "READ" : "WRITE") << ANSI_COLOR_DEFAULT;
    os << " S:" << format(state.SReg());
    os << " E:" << format(state.EReg());
    os << " B:" << format(state.BReg());
    os << " TR:" << format(state.TRReg());
    os << " PDL:" << format(state.PDLReg());
    os << " H:" << format(state.HReg());
    os << " CP:" << format(state.CPReg());
    os << " SP:" << state.SPReg();
    os << " PC:" << format(state.PCReg()) << std::endl;
    os << ANSI_COLOR_B_YELLOW << state.m_ArgumentRegisters << ANSI_COLOR_DEFAULT << std::endl;

    // Print heap
    for (size_t i = 0; i < state.m_Heap.size(); i++)
    {
        os << "\t"
           << "H" << i << " " << *(state.m_Heap[i]) << std::endl;
    }

    // Print stack
    for (size_t i = 0; i < state.m_Stack.size(); i++)
    {
        os << "Environment: " << i << std::endl;
        os << *(state.m_Stack[i]) << std::endl;
    }

    // Print trail
    for (size_t i = 0; i < state.m_Trail.size(); i++)
    {
        os << "\t"
           << "TR" << i << " " << *(state.m_Trail[i]) << std::endl;
    }
    return os;
}