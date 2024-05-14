#pragma once

#include <set>
#include <string>
#include <memory>
#include <unordered_map>

#include "../wam_code/instruction/Instructions.hpp"
#include "../wam_code/WAMCode.hpp"

using Label = std::string;

struct TableEntry
{
    size_t m_Generated = 0;
    size_t m_Clauses = 1;
};

enum CodeGenerationMode
{
    HEAD,
    BODY
};

class CompilationContext
{
public:
    void add(const std::string &symbol,
             std::shared_ptr<TableEntry> entry);
    std::shared_ptr<TableEntry> get(const std::string &symbol);

    void addInstruction(std::shared_ptr<Instruction> instr);
    void addInstructions(const std::vector<std::shared_ptr<Instruction>> &instructions);

    void addLabel(const Label &label);
    size_t getLabelAddress(const Label &label);

    WAMCode code(void);

    /**
     * @brief Update jump instructions in WAMCode
     */
    void updateJumpInstructions(void);

    /**
     * @brief Generates a temporary variable used in unification of complex objects
     * @return String in form of __Tn, where n is the number of currently allocated variables for the clause.
     */
    std::string generateTempVar(void);

    /**
     * @return Number of allocated variables
     */
    size_t allocated(void);

    /**
     * @brief Adds a variable to the WAMCode instance, used for user-entered variables
     * @param variable Name of the variable to be added
     */
    void addVariable(const std::string &variable);

    /**
     * @brief Notes a variable, used in the compilation process, used in bytecode generation.
     * @param variable Name of the variable to be added
     */
    void noteVariable(const std::string &variable);

    /***
     * @brief Returns the offset of a variable in th current environment
     * @param variable Name of a variable
     * @return Variable's offset in the current environment
     */
    size_t getVarOffset(const std::string &variable);

    /**
     * @brief Resets all variables stored in the context, used in the beginning of a clause-level compilation.
     */
    void resetVariables(void);

    /**
     * @return Returns next available register to be used in bytecode generation.
     */
    size_t availableReg(void);

    /**
     * @brief Sets the next available register to a value specified by the parameter
     * @param reg Register to be set as next available
     */
    void setAvailableReg(size_t reg);

    /**
     * @return Returns the name of the next available arithmetic variable, used in is operator bytecode generation.
     */
    std::string getAvailableArithmeticVariable(void);

    /**
     * @brief Resets the next avilable arithmetic variable to the default value 0
     */

    /**
     * @brief Increases the next avilable arithmetic variable by one
     */
    void resetAvailableArithmeticVariable(void);
    void incrementAvailableArithmeticVariable(void);

    /**
     * @return The current mode that the clause's compilation process is currently in
     */
    CodeGenerationMode mode(void);

    /**
     * @brief Set the compilation mode to HEAD
     */
    void setHeadGenerationMode(void);

    /**
     * @brief Set the compilation mode to BODY
     */
    void setBodyGenerationMode(void);

private:
    size_t m_AvailableArithmeticVariable = 0;
    size_t m_AvailableRegister = 1;
    std::unordered_map<std::string, std::shared_ptr<TableEntry>> m_SymbolTable;
    WAMCode m_GeneratedCode;

    // Used in Clause-Level compilation
    std::map<std::string, size_t> m_Variables;
    CodeGenerationMode m_CGMode = HEAD;
};