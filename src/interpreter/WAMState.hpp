#pragma once

#include "data_structures/Word.hpp"
#include "data_structures/ChoicePoint.hpp"
#include "data_structures/ArgumentRegisters.hpp"

#include <stack>
#include <vector>
#include <cstdlib>

const size_t UNSET_REG = -1;

struct WAMState
{
    ~WAMState(void);
    void fillRegister(Word *word, size_t reg);

    size_t SReg(void);
    size_t TRReg(void);
    size_t PDLReg(void);
    size_t SPReg(void);
    size_t HReg(void);

    void setWriteMode(void);
    void setReadMode(void);
    bool readMode(void);

    // Heap operations
    void heapPush(Word *word);
    void heapPop(void);
    Word *heapTop(void);

    // Choice Point Stack operations
    void stackPush(ChoicePoint *cp);
    void stackPop(void);
    bool stackEmpty(void);
    ChoicePoint *stackTop(void);
    ChoicePoint *getChoicePoint(size_t address);

    // Trail operations
    void trailPush(VariableWord *word);
    void trailPop(void);
    VariableWord *trailTop(void);

    // PDL operations
    void pdlPush(void);
    void pdlPop(void);
    void pldTop(void);

    friend std::ostream &operator<<(std::ostream &os, const WAMState &state);

    size_t m_StructurePointer;

    size_t m_ProgramCounter = UNSET_REG;
    size_t m_ContinuationPointer = UNSET_REG;

    size_t m_BacktrackRegister = UNSET_REG;
    size_t m_EnvironmentRegister = UNSET_REG;

    std::vector<Word *> m_Heap;
    std::vector<VariableWord *> m_Trail;
    std::vector<ChoicePoint *> m_Stack; // Represented as a vector because I need to have a random access available
    std::vector<Word *> m_PushDownList;
    ArgumentRegisters m_ArgumentRegisters;

    bool m_ReadMode = false;
    bool m_FailFlag = false;
};