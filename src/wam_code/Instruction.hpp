#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "../interpreter/WAMState.hpp"

#include <ostream>
#include <string>

struct Instruction
{
    virtual Instruction *clone(void) = 0;
    virtual ~Instruction(void) = default;
    virtual void execute(WAMState &state) = 0;
    virtual void print(std::ostream &os) = 0;
};

struct BranchInstruction : public Instruction
{
    BranchInstruction(const std::string & label, size_t address = 0);
    void setAddress(size_t address);

    std::string m_Label;
    size_t m_Address = 0;
};

// Indexing instructions
struct MarkInstruction : public Instruction
{
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) override;
};

struct RetryMeElseInstruction : public BranchInstruction
{
    RetryMeElseInstruction(const std::string &label, size_t address = 0);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) override;
};

struct BacktrackInstruction : public Instruction
{
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) override;
};

struct FailInstruction : public Instruction
{
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) override;
};

// Procedural Instructions
struct AllocateInstruction : public Instruction
{
    AllocateInstruction(size_t n);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) override;
    size_t m_N;
};

struct CallInstruction : public BranchInstruction
{
    CallInstruction(const std::string &label, size_t address = 0);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) override;
};

struct ReturnInstruction : public Instruction
{
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) override;
};

// Get Instructions
struct GetInstruction : public Instruction
{
protected:
    GetInstruction(const std::string &name, size_t argumentRegister);
    std::string m_Name;
    size_t m_ArgumentRegister;
};

struct GetConstantInstruction : public GetInstruction
{
    GetConstantInstruction(const std::string &name, size_t argumentRegister);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) override;
};

struct GetListInstruction : public GetInstruction
{
};

struct GetStructureInstruction : public GetInstruction
{
    GetStructureInstruction(const std::string &name, size_t argumentRegister);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) override;
};

struct GetVariableInstruction : public GetInstruction
{
    GetVariableInstruction(const std::string &name, size_t argumentRegister);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) override;
};
// Put Instructions
struct PutInstruction : public Instruction
{
protected:
    PutInstruction(const std::string &name, size_t argumentRegister);
    std::string m_Name;
    size_t m_ArgumentRegister;
};
struct PutConstantInstruction : public PutInstruction
{
    PutConstantInstruction(const std::string &name, size_t argumentRegister);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) override;
};

struct PutVariableInstruction : public PutInstruction
{
    PutVariableInstruction(const std::string &name, size_t argumentRegister);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) override;
};

struct PutStructureInstruction : public PutInstruction
{
};

// Unify Instructions

struct UnifyInstruction : public Instruction
{
protected:
    UnifyInstruction(const std::string &name);
    std::string m_Name;
};

struct UnifyConstantInstruction : public UnifyInstruction
{
    UnifyConstantInstruction(const std::string &name);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) override;
};

struct UnifyVariableInstruction : public UnifyInstruction
{
    UnifyVariableInstruction(const std::string &name);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) override;
};

#endif // INSTRUCTION_H