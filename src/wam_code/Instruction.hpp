#pragma once

#include "../interpreter/WAMState.hpp"

#include <ostream>
#include <string>

struct Instruction
{
    virtual Instruction *clone(void) = 0;
    virtual ~Instruction(void) = default;
    virtual void execute(WAMState &state) = 0;
    virtual void print(std::ostream &os) const = 0;

    friend std::ostream &operator<<(std::ostream &os, const Instruction &instr);
};

struct BranchInstruction : public Instruction
{
    BranchInstruction(const std::string &label, size_t address = 0);
    void setAddress(size_t address);

    std::string m_Label;
    size_t m_Address = 0;
};

// Indexing instructions
struct MarkInstruction : public Instruction
{
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
};

struct RetryMeElseInstruction : public BranchInstruction
{
    RetryMeElseInstruction(const std::string &label, size_t address = 0);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
};

struct BacktrackInstruction : public Instruction
{
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
};

struct FailInstruction : public Instruction
{
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
};

// Procedural Instructions
struct AllocateInstruction : public Instruction
{
    AllocateInstruction(size_t n);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
    size_t m_N;
};

struct CallInstruction : public BranchInstruction
{
    CallInstruction(const std::string &label, size_t address = 0);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
};

struct ReturnInstruction : public Instruction
{
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
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
    void print(std::ostream &os) const override;
};

struct GetListInstruction : public GetInstruction
{
    GetListInstruction(const std::string &name, size_t argumentRegister);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
};

struct GetStructureInstruction : public GetInstruction
{
    GetStructureInstruction(const std::string &name, size_t argumentRegister, size_t arity);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;

    size_t m_Arity;
};

struct GetVariableInstruction : public GetInstruction
{
    GetVariableInstruction(const std::string &name, size_t argumentRegister, size_t offset);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;

    size_t m_Offset;
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
    void print(std::ostream &os) const override;
};

struct PutVariableInstruction : public PutInstruction
{
    PutVariableInstruction(const std::string &name, size_t argumentRegister, size_t offset);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;

    size_t m_Offset;
};

struct PutListInstruction : public PutInstruction
{
    PutListInstruction(const std::string &name, size_t ArgumentRegister);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
};

struct PutStructureInstruction : public PutInstruction
{
    PutStructureInstruction(const std::string &name, size_t argumentRegister, size_t arity);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;

    size_t m_Arity;
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
    void print(std::ostream &os) const override;
};

struct UnifyVariableInstruction : public UnifyInstruction
{
    UnifyVariableInstruction(const std::string &name);
    Instruction *clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
};