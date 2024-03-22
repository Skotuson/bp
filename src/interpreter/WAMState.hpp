#pragma once

#include "data_structures/Word.hpp"
#include "data_structures/ChoicePoint.hpp"
#include "data_structures/ArgumentRegisters.hpp"

#include <stack>
#include <tuple>
#include <vector>
#include <cstdlib>

using PDLTriple = std::tuple<size_t, size_t, size_t>;

const size_t UNSET_REG = -1;

struct WAMState
{
    ~WAMState(void);
    void fillRegister(Word *word, size_t reg);

    size_t SReg(void) const;
    size_t TRReg(void) const;
    size_t PDLReg(void) const;
    size_t SPReg(void) const;
    size_t HReg(void) const;

    void setWriteMode(void);
    void setReadMode(void);
    bool readMode(void) const;

    // Heap operations
    void heapPush(Word *word);
    void heapPop(void);
    Word *heapTop(void);
    Word *heapAt(size_t offset);

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
    void pdlPush(const PDLTriple & pdlTriple);
    void pdlPop(void);
    PDLTriple pdlTop(void);

    friend std::ostream &operator<<(std::ostream &os, const WAMState &state);

    size_t m_StructurePointer = 0;

    size_t m_ProgramCounter = UNSET_REG;
    size_t m_ContinuationPointer = UNSET_REG;

    size_t m_BacktrackRegister = UNSET_REG;
    size_t m_EnvironmentRegister = UNSET_REG;

    std::vector<Word *> m_Heap;
    std::vector<VariableWord *> m_Trail;
    std::vector<ChoicePoint *> m_Stack; // Represented as a vector because I need to have a random access available
    std::vector<PDLTriple> m_PushDownList;
    ArgumentRegisters m_ArgumentRegisters;

    bool m_ReadMode = false;
    bool m_FailFlag = false;
};