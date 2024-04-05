#pragma once

#include "Instruction.hpp"

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

    void addInstructions(const std::vector<std::shared_ptr<Instruction>> &instructions);
    void deleteInstruction(size_t idx);
    void popInstructions(size_t n);

    void updateJumpInstructions(void);

    void merge(const WAMCode &code);

    std::shared_ptr<Instruction> getInstruction(size_t pc) const;

    void dumpInstruction(size_t pc, std::ostream &os) const;
    void dump(std::ostream &os);

    void addVariable(const Variable &v);
    std::map<size_t, std::string> getVariables(void);

    void addLabel(const Label &label);
    void removeLabel(const Label &label);
    size_t getLabelAddress(const Label &label);

    std::map<size_t, Label> m_AddressToLabel;
    std::map<Label, size_t> m_LabelToAddress;
    std::vector<std::shared_ptr<Instruction>> m_Program;
    std::map<size_t, std::string> m_Variables;
};