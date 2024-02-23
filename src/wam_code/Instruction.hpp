#ifndef INSTRUCTION_H
#define INSTRUCTION_H

struct Instruction
{
    // virtual void execute(void);
    // virtual void print(std::ostream & os);
};

// Indexing instructions
struct MarkInstruction : public Instruction
{
};

struct RetryMeElseInstruction : public Instruction
{
};

struct BacktrackInstruction : public Instruction
{
};

// Procedural Instructions
struct CallInstruction : public Instruction
{
};

struct ReturnInstruction : public Instruction
{
};

// Get Instructions
struct GetConstantInstruction : public Instruction
{
};

struct GetListInstruction : public Instruction
{
};

struct GetStructureInstruction : public Instruction
{
};
// Put Instructions
// Unify Instructions

#endif // INSTRUCTION_H