#pragma once

#include "Instruction.hpp"

#include <map>
#include <vector>
#include <string>
#include <ostream>

using Label = std::string;
using Variable = std::string;

struct WAMCode
{
    WAMCode(void) = default;
    ~WAMCode(void);
    WAMCode(const WAMCode &wamCode);
    WAMCode &operator=(const WAMCode &wamCode);

    size_t size() const;

    void addInstructions(const std::vector<Instruction *> &instructions);
    void deleteInstruction(size_t idx);
    void popInstructions(size_t n);

    void updateJumpInstructions(void);

    void merge(const WAMCode &code);

    Instruction *getInstruction(size_t pc) const;

    void dumpInstruction(size_t pc, std::ostream &os) const;
    void dump(std::ostream &os);

    void addVariable(const Variable &v);
    std::vector<Variable> getVariables(void);

    void addLabel(const Label &label);
    void removeLabel(const Label &label);
    size_t getLabelAddress(const Label &label);

    std::map<size_t, Label> m_AddressToLabel;
    std::map<Label, size_t> m_LabelToAddress;
    std::vector<Instruction *> m_Program;
    std::vector<Variable> m_Variables;
};