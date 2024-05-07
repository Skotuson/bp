#include "Instruction.hpp"

#include "../../word/ListWord.hpp"

#include <iostream>
#include <cassert>
#include <memory>

const std::vector<std::vector<size_t>> Instruction::m_ClearPDLTable = {
    {1, 1, 1, 1, 1},
    {2, 3, 5, 5, 5},
    {2, 4, 6, 0, 0},
    {2, 4, 0, 7, 0},
    {2, 4, 0, 0, 8}};

void Instruction::fail(WAMState &state)
{
    std::shared_ptr<FailInstruction> fail = std::make_shared<FailInstruction>();
    fail->execute(state);
}

void Instruction::clearPDL(WAMState &state, std::shared_ptr<Word> X, std::shared_ptr<Word> Y)
{
    // Empty the PDL
    while (!state.pdlEmpty())
    {
        state.pdlPop();
    }

    while (42)
    {
        size_t branch = m_ClearPDLTable[X->tag()][Y->tag()];
        // X is a ref, dereference:
        if (branch == 1)
        {
            X = X->dereference();
            continue;
        }

        // Y is a ref, dereference:
        else if (branch == 2)
        {
            Y = Y->dereference();
            continue;
        }

        // X and Y are both unbound variables
        else if (branch == 3)
        {
            std::shared_ptr<VariableWord> x = std::static_pointer_cast<VariableWord>(X);
            std::shared_ptr<VariableWord> y = std::static_pointer_cast<VariableWord>(Y);
            // Trail both X and Y
            state.trailPush(x);
            state.trailPush(y);
            // Bind them together
            y->bind(X);
        }

        // X is not a variable, Y is an unbound variable
        else if (branch == 4)
        {
            std::shared_ptr<VariableWord> vw = std::static_pointer_cast<VariableWord>(Y);
            // Trail
            state.trailPush(vw);
            vw->bind(X->clone());
        }

        // Y is a constant, X is an unbound variable
        else if (branch == 5)
        {
            std::shared_ptr<VariableWord> vw = std::static_pointer_cast<VariableWord>(X);
            // Trail
            state.trailPush(vw);
            vw->bind(Y->clone());
        }

        // Both X and Y are constants
        else if (branch == 6)
        {
            std::shared_ptr<ConstantWord> xc = std::static_pointer_cast<ConstantWord>(X);
            if (!Y->compareToConst(xc))
            {
                fail(state);
            }
        }

        else if (branch == 7)
        {
            size_t XA = std::static_pointer_cast<ListWord>(X)->m_HeapAddress;
            size_t YA = std::static_pointer_cast<ListWord>(Y)->m_HeapAddress;
            state.pdlPush({XA, YA, 2});
        }

        else if (branch == 8)
        {
            std::shared_ptr<StructurePointerWord> x = std::static_pointer_cast<StructurePointerWord>(X);
            std::shared_ptr<StructurePointerWord> y = std::static_pointer_cast<StructurePointerWord>(Y);
            std::shared_ptr<StructureWord> xs = std::static_pointer_cast<StructureWord>(state.heapAt(x->m_HeapAddress));
            std::shared_ptr<StructureWord> ys = std::static_pointer_cast<StructureWord>(state.heapAt(y->m_HeapAddress));

            if (!xs->compareToStruct(ys))
            {
                fail(state);
                break;
            }

            state.pdlPush({x->m_HeapAddress + 1, y->m_HeapAddress + 1, xs->m_Arity});
        }

        // Fail branch
        else
        {
            fail(state);
            break;
        }

        // If PDL is empty, terminate the loop
        if (state.pdlEmpty())
        {
            break;
        }

        // Get information from the PDL and repeat the loop
        else
        {
            PDLTriple pdlTriple = state.pdlTop();
            size_t XA = std::get<0>(pdlTriple);
            size_t YA = std::get<1>(pdlTriple);
            X = state.heapAt(XA);
            Y = state.heapAt(YA);
            size_t N = std::get<2>(pdlTriple) - 1;
            state.pdlPop();

            if (N)
            {
                state.pdlPush({XA + 1, YA + 1, N});
            }
        }
    }
}

std::ostream &operator<<(std::ostream &os, const Instruction &instr)
{
    instr.print(os);
    return os;
}

std::shared_ptr<Instruction> FailInstruction::clone(void)
{
    return std::make_shared<FailInstruction>();
}

void FailInstruction::execute(WAMState &state)
{
    std::shared_ptr<ChoicePoint> cp = state.stackAt(state.m_BacktrackRegister);
    if (cp)
    {
        // Reload argument registers
        state.m_ArgumentRegisters = cp->m_ArgumentRegisters;

        // Reset heap
        while (state.HReg() != cp->m_BH)
        {
            state.heapPop();
        }

        // Reset all variables instantiated since choice point was built
        while (state.TRReg() != cp->m_BTR)
        {
            std::shared_ptr<VariableWord> popped = state.trailTop();
            popped->unbind();
            *popped->ref() = popped;
            state.trailPop();
        }
        // Branch to next rule
        state.m_ProgramCounter = cp->m_FA;
    }
    // TODO: Experimental (check when choice point stack is empty)
    else if (state.m_BacktrackRegister == UNSET_REG)
        state.m_FailFlag = true;
}

void FailInstruction::print(std::ostream &os) const
{
    os << "__fail__";
}