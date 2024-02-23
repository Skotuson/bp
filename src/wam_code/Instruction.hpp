#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <ostream>

struct Instruction
{
    // virtual void execute(void) = 0;
    virtual void print(std::ostream & os) = 0;
};

// Indexing instructions
struct MarkInstruction : public Instruction
{
    void print(std::ostream &os) override;
};

struct RetryMeElseInstruction : public Instruction
{
    void print(std::ostream &os) override;
};

struct BacktrackInstruction : public Instruction
{
};

// Procedural Instructions
struct CallInstruction : public Instruction
{
    void print(std::ostream &os) override;
};

struct ReturnInstruction : public Instruction
{
};

// Get Instructions
struct GetConstantInstruction : public Instruction
{
    void print(std::ostream &os) override;
};

struct GetListInstruction : public Instruction
{
};

struct GetStructureInstruction : public Instruction
{
};

struct GetVariableInstruction : public Instruction
{
    void print(std::ostream &os) override;
};
// Put Instructions
// Unify Instructions

#endif // INSTRUCTION_H