#pragma once

#include "ArgumentRegisters.hpp"
#include "../../word/Word.hpp"
#include <vector>

struct ChoicePoint
{
    ChoicePoint(const ArgumentRegisters &argReg, size_t bce, size_t bcp, size_t bb, size_t btr, size_t bh, size_t fa);

    const ArgumentRegisters &argumentRegisters(void);

    size_t BCEField(void);
    size_t BCPField(void);
    size_t BBField(void);
    size_t BTRField(void);
    size_t BHField(void);
    size_t FAField(void);

    void setBCEField(size_t bce);
    void setBCPField(size_t bcp);
    void setBBField(size_t bb);
    void setBTRField(size_t btr);
    void setBHField(size_t bh);
    void setFAField(size_t fa);

    std::vector<std::shared_ptr<Word>> &localVariables(void);

    friend std::ostream &operator<<(std::ostream &os, const ChoicePoint &cp);

private:
    // Copy of the argument registers
    ArgumentRegisters m_ArgumentRegisters;
    // Backtrack Continuation Environment
    size_t m_BCE;
    // Backtrack Continuation Pointer
    size_t m_BCP;
    // Backtrack B (register)
    size_t m_BB;
    // Backtrack Trail Top
    size_t m_BTR;
    // Backtrack Heap Top
    size_t m_BH;
    // Failure Address
    size_t m_FA;
    // Variables for the local environment
    std::vector<std::shared_ptr<Word>> m_Variables;
};