#include "WAMState.hpp"

WAMState::~WAMState(void)
{
    for (auto word : m_Trail)
        delete word;
    for (auto cp : m_Stack)
        delete cp;
}

void WAMState::fillRegister(Word *word, size_t reg)
{
    m_ArgumentRegisters.fillRegister(word, reg);
}

size_t WAMState::SReg(void)
{
    return m_Stack.size() - 1;
}

size_t WAMState::TRReg(void)
{
    return m_Trail.size();
}

void WAMState::stackPush(ChoicePoint *cp)
{
    m_Stack.push_back(cp);
}

void WAMState::stackPop(void)
{
    if (!m_Stack.empty())
    {
        delete m_Stack.back();
        m_Stack.pop_back();
    }
}

bool WAMState::stackEmpty(void)
{
    return m_Stack.empty();
}

ChoicePoint *WAMState::stackTop(void)
{
    if (m_Stack.size())
    {
        return m_Stack.back();
    }

    return nullptr;
}

ChoicePoint *WAMState::getChoicePoint(size_t address)
{
    if (m_Stack.size() && address < m_Stack.size())
    {
        return m_Stack[address];
    }

    return nullptr;
}

std::ostream &operator<<(std::ostream &os, const WAMState &state)
{
    os << "STACK-BOT" << std::endl;
    size_t n = 1;
    for (const auto &cp : state.m_Stack)
    {
        std::cout << "Stack pos: " << n++ << std::endl;
        os << *cp << std::endl;
    }
    os << "STACK-TOP";
    return os;
}