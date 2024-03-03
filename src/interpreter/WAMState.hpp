#ifndef WAMSTATE_H
#define WAMSTATE_H

#include "data_structures/Word.hpp"
#include "data_structures/ChoicePoint.hpp"
#include "data_structures/ArgumentRegisters.hpp"

#include <stack>
#include <vector>
#include <cstdlib>

struct WAMState
{
    void fillRegister(Word *word, size_t reg);

    size_t SReg(void);
    size_t TRReg(void);

    void stackPush(ChoicePoint *cp);
    void stackPop(void);
    bool stackEmpty(void);
    ChoicePoint *stackTop(void);
    ChoicePoint *getChoicePoint(size_t address);

    friend std::ostream &operator<<(std::ostream &os, const WAMState &state);

    size_t m_ProgramCounter = 0;
    size_t m_ContinuationPointer = 0;

    size_t m_BacktrackRegister = 0;
    size_t m_EnvironmentRegister = 0;

    std::vector<Word *> m_Trail;
    std::vector<ChoicePoint *> m_Stack; // Represented as a vector because I need to have a random access available
    ArgumentRegisters m_ArgumentRegisters;

    bool m_ModeFlag = false;
};

#endif // WAMSTATE_H