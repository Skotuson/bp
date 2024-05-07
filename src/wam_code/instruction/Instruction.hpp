#pragma once

#include "../../interpreter/WAMState.hpp"

#include <ostream>
#include <string>

struct Instruction
{
    virtual std::shared_ptr<Instruction> clone(void) = 0;
    virtual ~Instruction(void) = default;
    virtual void execute(WAMState &state) = 0;
    virtual void print(std::ostream &os) const = 0;

    static void fail(WAMState &state);
    static void clearPDL(WAMState &state, std::shared_ptr<Word> X, std::shared_ptr<Word> Y);

    static const std::vector<std::vector<size_t>> m_ClearPDLTable;

    friend std::ostream &operator<<(std::ostream &os, const Instruction &instr);
};

struct FailInstruction : public Instruction
{
    std::shared_ptr<Instruction> clone(void) override;
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

struct GetStructureInstruction : public GetInstruction
{
    GetStructureInstruction(const std::string &name, size_t argumentRegister, size_t arity);
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;

    size_t m_Arity;
};

struct GetVariableInstruction : public GetInstruction
{
    GetVariableInstruction(const std::string &name, size_t argumentRegister, size_t offset);
    std::shared_ptr<Instruction> clone(void) override;
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
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
};

struct PutVariableInstruction : public PutInstruction
{
    PutVariableInstruction(const std::string &name, size_t argumentRegister, size_t offset);
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;

    size_t m_Offset;
};

struct PutListInstruction : public PutInstruction
{
    PutListInstruction(const std::string &name, size_t argumentRegister);
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
};

struct PutStructureInstruction : public PutInstruction
{
    PutStructureInstruction(const std::string &name, size_t argumentRegister, size_t arity);
    std::shared_ptr<Instruction> clone(void) override;
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
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;
};

struct UnifyVariableInstruction : public UnifyInstruction
{
    UnifyVariableInstruction(const std::string &name, size_t offset);
    std::shared_ptr<Instruction> clone(void) override;
    void execute(WAMState &state) override;
    void print(std::ostream &os) const override;

    size_t m_Offset;
};