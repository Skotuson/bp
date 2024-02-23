#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <ostream>
#include <string>

struct Instruction
{
    // virtual void execute(void) = 0;
    virtual void print(std::ostream &os) = 0;
};

// Indexing instructions
struct MarkInstruction : public Instruction
{
    void print(std::ostream &os) override;
};

struct RetryMeElseInstruction : public Instruction
{
    RetryMeElseInstruction(const std::string &label);
    void print(std::ostream &os) override;

    std::string m_Label;
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
    void print(std::ostream &os) override;
};

struct GetListInstruction : public GetInstruction
{
};

struct GetStructureInstruction : public GetInstruction
{
};

struct GetVariableInstruction : public GetInstruction
{
    GetVariableInstruction(const std::string &name, size_t argumentRegister);
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
    void print(std::ostream &os) override;
};

struct PutVariableInstruction : public PutInstruction
{
    PutVariableInstruction(const std::string &name, size_t argumentRegister);
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
    void print(std::ostream &os) override;
};

struct UnifyVariableInstruction : public UnifyInstruction
{
    UnifyVariableInstruction(const std::string &name);
    void print(std::ostream &os) override;
};

#endif // INSTRUCTION_H