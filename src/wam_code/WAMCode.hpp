#pragma once

#include "instruction/Instruction.hpp"

#include <map>
#include <vector>
#include <memory>
#include <string>
#include <ostream>

using Label = std::string;
using Variable = std::pair<size_t, std::string>;

const size_t BAD_ADDRESS = -1;

struct WAMCode
{
    WAMCode(void) = default;
    WAMCode(const WAMCode &wamCode);
    WAMCode &operator=(const WAMCode &wamCode);

    size_t size() const;

    /**
     * @brief Adds instructions to the code
     * @param instructions Instructions to be added
     */
    void addInstructions(const std::vector<std::shared_ptr<Instruction>> &instructions);

    /**
     * @brief Removes the last n instructions
     * @param n Number of instructions to remove
     */
    void popInstructions(size_t n);

    /**
     * @brief Iterates through the instructions and sets addresses of branch instructions
     */
    void updateJumpInstructions(void);

    /**
     * @brief Merges current code with another code and updates jump instructions
     */
    void merge(const WAMCode &code);

    /**
     * @param pc Index of the instruction to get
     * @return Instruction at the index specified by the parametr
     */
    std::shared_ptr<Instruction> getInstruction(size_t pc) const;

    /**
     * @brief Prints out an instruction. If the instruction is on the same address as label, prints the label aswell
     * @param pc Instruction to print
     * @param os Output stream to print the instruction to
     */
    void dumpInstruction(size_t pc, std::ostream &os) const;

    /**
     * @brief Prints out all instructions
     * @param os Output stream to print the instruction to
     */
    void dump(std::ostream &os);

    /**
     * @brief Stores a variable
     * @param v Variable to add
     */
    void addVariable(const Variable &v);

    /**
     * @return Variable mapping in the form of offset -> variable name
     */
    std::map<size_t, std::string> getVariables(void) const;

    /**
     * @brief Records a label in form of mapping address -> name of the label and vice versa
     * @param label Label to add
     */
    void addLabel(const Label &label);

    /**
     * @brief Deletes label
     * @param label Label to delete
     */
    void removeLabel(const Label &label);

    /**
     * @return Label address
     */
    size_t getLabelAddress(const Label &label);

private:
    std::map<size_t, Label> m_AddressToLabel;
    std::map<Label, size_t> m_LabelToAddress;
    std::vector<std::shared_ptr<Instruction>> m_Program;
    std::map<size_t, std::string> m_Variables;
};