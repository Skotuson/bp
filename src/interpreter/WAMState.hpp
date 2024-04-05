#pragma once

#include "data_structures/word/Word.hpp"
#include "data_structures/ChoicePoint.hpp"
#include "data_structures/word/ConstantWord.hpp"
#include "data_structures/word/VariableWord.hpp"
#include "data_structures/ArgumentRegisters.hpp"
#include "data_structures/word/StructureWord.hpp"
#include "data_structures/word/StructurePointerWord.hpp"

#include <map>
#include <stack>
#include <tuple>
#include <memory>
#include <vector>
#include <cstdlib>

using PDLTriple = std::tuple<size_t, size_t, size_t>;

const size_t UNSET_REG = -1;

struct WAMState
{
    void fillRegister(std::shared_ptr<Word> word, size_t reg);

    size_t SReg(void) const;
    size_t TRReg(void) const;
    size_t PDLReg(void) const;
    size_t SPReg(void) const;
    size_t HReg(void) const;

    void setWriteMode(void);
    void setReadMode(void);
    bool readMode(void) const;

    // Heap operations
    void heapPush(std::shared_ptr<Word> word);
    void heapPop(void);
    std::shared_ptr<Word> heapTop(void);
    std::shared_ptr<Word> heapAt(size_t offset);

    // Choice Point Stack operations
    void stackPush(std::shared_ptr<ChoicePoint> cp);
    void stackPop(void);
    bool stackEmpty(void);
    std::shared_ptr<ChoicePoint> stackTop(void);
    std::shared_ptr<ChoicePoint> getChoicePoint(size_t address);

    // Trail operations
    void trailPush(std::shared_ptr<VariableWord> word);
    void trailPop(void);
    std::shared_ptr<VariableWord> trailTop(void);

    // PDL operations
    void pdlPush(const PDLTriple &pdlTriple);
    void pdlPop(void);
    bool pdlEmpty(void);
    PDLTriple pdlTop(void);

    std::string variableToString(size_t choicePoint, size_t offset);

    friend std::ostream &operator<<(std::ostream &os, const WAMState &state);

    size_t m_StructurePointer = 0;

    size_t m_ProgramCounter = UNSET_REG;
    size_t m_ContinuationPointer = UNSET_REG;

    size_t m_BacktrackRegister = UNSET_REG;
    size_t m_EnvironmentRegister = UNSET_REG;

    std::vector<std::shared_ptr<Word>> m_Heap;
    std::vector<std::shared_ptr<VariableWord>> m_Trail;
    std::vector<std::shared_ptr<ChoicePoint>> m_Stack; // Represented as a vector because I need to have a random access available
    std::vector<PDLTriple> m_PushDownList;
    ArgumentRegisters m_ArgumentRegisters;

    std::map<size_t, std::string> m_QueryVariables;

    bool m_ReadMode = false;
    bool m_FailFlag = false;
};