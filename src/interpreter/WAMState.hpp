#pragma once

#include "../word/Word.hpp"
#include "data_structures/ChoicePoint.hpp"
#include "../word/ConstantWord.hpp"
#include "../word/VariableWord.hpp"
#include "data_structures/ArgumentRegisters.hpp"
#include "../word/StructureWord.hpp"
#include "../word/StructurePointerWord.hpp"

#include <map>
#include <stack>
#include <tuple>
#include <memory>
#include <vector>
#include <cstdlib>

using PDLTriple = std::tuple<size_t, size_t, size_t>;

const size_t UNSET_REG = -2;

struct WAMState
{
    void fillRegister(std::shared_ptr<Word> word, size_t reg);

    size_t SReg(void) const;
    size_t EReg(void) const;
    size_t BReg(void) const;
    size_t TRReg(void) const;
    size_t PDLReg(void) const;
    size_t SPReg(void) const;
    size_t HReg(void) const;
    size_t CP(void) const;
    size_t PC(void) const;

    void setWriteMode(void);
    void setReadMode(void);
    bool readMode(void) const;

    bool fail(void) const;
    void setFailFlag(bool failFlag);
    bool halt(void) const;
    void setHaltFlag(bool failFlag);

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
    std::shared_ptr<ChoicePoint> stackAt(size_t address);

    // Trail operations
    void trailPush(std::shared_ptr<VariableWord> word);
    void trailPop(void);
    std::shared_ptr<VariableWord> trailTop(void);
    std::shared_ptr<VariableWord> trailAt(size_t address);

    // PDL operations
    void pdlPush(const PDLTriple &pdlTriple);
    void pdlPop(void);
    bool pdlEmpty(void);
    PDLTriple pdlTop(void);

    std::string variableToString(size_t offset, size_t choicePoint = 0);

    size_t getAllocatedVariables(void);
    void setAllocatedVariables(size_t allocatedVariables);

    const ArgumentRegisters &getArgumentRegisters(void);
    void setArgumentRegisters(const ArgumentRegisters &argumentRegisters);
    std::shared_ptr<Word> dereferenceArgumentRegister(size_t argReg);

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

    std::map<size_t, std::string> m_QueryVariables;
    std::map<size_t, std::shared_ptr<VariableWord>> m_QueryWords;

private:
    ArgumentRegisters m_ArgumentRegisters;
    bool m_ReadMode = false;
    bool m_FailFlag = false;
    bool m_HaltFlag = false;

    size_t m_AllocatedVariables = 0;
};