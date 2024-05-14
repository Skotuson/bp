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
    /**
     * @brief Loads a argument register
     * @param word Word to load into argument register
     * @param reg Register to load
     */
    void fillRegister(std::shared_ptr<Word> word, size_t reg);

    // Methods for obtaining values from individual machine registers
    size_t SReg(void) const;
    size_t EReg(void) const;
    size_t BReg(void) const;
    size_t TRReg(void) const;
    size_t PDLReg(void) const;
    size_t SPReg(void) const;
    size_t HReg(void) const;
    size_t CPReg(void) const;
    size_t PCReg(void) const;

    // Methods for setting values to individual machine registers
    void setEReg(size_t ereg);
    void setBReg(size_t breg);
    void setSPReg(size_t spreg);
    void setCPReg(size_t cpreg);
    void setPCReg(size_t pcreg);

    // Methods for setting a retrieving values from the mode flag
    void setWriteMode(void);
    void setReadMode(void);
    bool readMode(void) const;

    /**
     * @return True if fail flag is set
     */
    bool fail(void) const;
    /**
     * @brief Sets the fail flag
     * @param failFlag boolean value to set the fail flag to
     */
    void setFailFlag(bool failFlag);

    /**
     * @return True if halt flag is set
     */
    bool halt(void) const;
    /**
     * @brief Sets the halt flag
     * @param haltFlag boolean value to set the halt flag to
     */
    void setHaltFlag(bool haltFlag);

    // Heap operations
    void heapPush(std::shared_ptr<Word> word);
    void heapPop(void);
    std::shared_ptr<Word> heapTop(void);
    std::shared_ptr<Word> heapAt(size_t offset);
    // Used for printing
    const std::vector<std::shared_ptr<Word>> &heapRef(void);

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

    /**
     * @brief Gets a string representation of the value of a specific variable
     * @param offset Offset of the variable in the environment
     * @param choicePoint The environment, 0 by default
     * @return String representation of the variable
     */
    std::string variableToString(size_t offset, size_t choicePoint = 0);

    /**
     * @return Number of the non-query variables allocated so far, used to avoid naming collisions
     */
    size_t getAllocatedVariables(void);

    /**
     * @brief Sets the m_AllocatedVariables to a specific value
     * @param allocatedVariables Number to set the number of the next available variable to be used for non-query variables
     */
    void setAllocatedVariables(size_t allocatedVariables);

    const ArgumentRegisters &getArgumentRegisters(void);
    void setArgumentRegisters(const ArgumentRegisters &argumentRegisters);
    
    /**
     * @brief Dereferences a specific register. If a reference is encountered, it is chased until something other than a reference is encountered.
     * @param argReg Number of register to dereference. As the registers are labeled from 1, this method accounts for it
     */
    std::shared_ptr<Word> dereferenceArgumentRegister(size_t argReg);
    
    void setQueryVariables(const std::map<size_t, std::string> &queryVariables);
    std::string getQueryVariableName(size_t offset);
    bool isQueryVariable(size_t offset);

    void addQueryWord(size_t offset, std::shared_ptr<VariableWord> queryWord);

    friend std::ostream &operator<<(std::ostream &os, const WAMState &state);

private:
    //-------------STATUS FLAGS-------------//
    bool m_ReadMode = false;
    bool m_FailFlag = false;
    bool m_HaltFlag = false;

    //-------------MACHINE REGISTERS-------------//
    size_t m_StructurePointer = 0;
    size_t m_ProgramCounter = UNSET_REG;
    size_t m_ContinuationPointer = UNSET_REG;
    size_t m_BacktrackRegister = UNSET_REG;
    size_t m_EnvironmentRegister = UNSET_REG;

    //-------------VARIABLES HANDLING-------------//
    // Used for naming non-query variables
    size_t m_AllocatedVariables = 0;
    // Query variables, used when printing
    std::map<size_t, std::string> m_QueryVariables;
    // Used when printing, mainly for infinite terms
    std::map<size_t, std::shared_ptr<VariableWord>> m_QueryWords;

    //-------------MAJOR DATA STRUCTURES-------------//
    ArgumentRegisters m_ArgumentRegisters;
    // Represented as a vector because I need to have a random access available
    std::vector<PDLTriple> m_PushDownList;
    std::vector<std::shared_ptr<ChoicePoint>> m_Stack;
    std::vector<std::shared_ptr<VariableWord>> m_Trail;
    std::vector<std::shared_ptr<Word>> m_Heap;
};