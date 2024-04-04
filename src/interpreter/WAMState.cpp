#include "WAMState.hpp"

WAMState::~WAMState(void)
{
    for (auto word : m_Trail)
        delete word;
}

void WAMState::fillRegister(Word *word, size_t reg)
{
    m_ArgumentRegisters.fillRegister(word, reg);
}

size_t WAMState::SReg(void) const
{
    return m_Stack.size() - 1;
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

void WAMState::heapPush(Word *word)
{
    m_Heap.push_back(word);
}

void WAMState::heapPop(void)
{
    Word *top = m_Heap.back();
    m_Heap.pop_back();
    delete top;
}

Word *WAMState::heapTop(void)
{
    return m_Heap.back();
}

Word *WAMState::heapAt(size_t offset)
{
    return m_Heap[offset];
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

std::shared_ptr<ChoicePoint> WAMState::getChoicePoint(size_t address)
{
    if (m_Stack.size() && address < m_Stack.size())
    {
        return m_Stack[address];
    }

    return nullptr;
}

void WAMState::trailPush(VariableWord *word)
{
    m_Trail.push_back(word);
}

void WAMState::trailPop(void)
{
    Word *word = trailTop();
    m_Trail.pop_back();
    delete word;
}

VariableWord *WAMState::trailTop(void)
{
    return m_Trail[TRReg() - 1];
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

std::string WAMState::variableToString(size_t choicePoint, size_t offset)
{
    auto cp = getChoicePoint(choicePoint);
    return cp->m_Variables[offset]->toString();
}

std::ostream &operator<<(std::ostream &os, const WAMState &state)
{
    os << "Mode: " << (state.readMode() ? "READ" : "WRITE") << std::endl;
    os << "SP: " << state.SPReg() << std::endl;
    os << state.m_ArgumentRegisters << std::endl;
    os << "HEAP-BOT" << std::endl;
    for (const auto &w : state.m_Heap)
    {
        os << *w << std::endl;
    }
    os << "HEAP-TOP" << std::endl;
    os << "STACK-BOT" << std::endl;
    size_t n = 1;
    for (const auto &cp : state.m_Stack)
    {
        std::cout << "Stack pos: " << n++ << std::endl;
        os << *cp << std::endl;
    }
    os << "STACK-TOP" << std::endl;
    os << "TRAIL-BOT" << std::endl;

    n = 1;
    for (const auto &w : state.m_Trail)
    {
        os << *w << std::endl;
    }
    os << "TRAIL-TOP";
    return os;
}