#include "Word.hpp"

std::ostream &operator<<(std::ostream &os, const TAG &tag)
{
    switch (tag)
    {
    case TAG::CONSTANT:
        return os << "Constant";
    case TAG::LIST:
        return os << "List";
    case TAG::REFERENCE:
        return os << "Reference";
    case TAG::S_POINTER:
        return os << "Structure Pointer";
    case TAG::VARIABLE:
        return os << "Variable";
    case TAG::STRUCTURE:
        return os << "Structure";
    }
    return os;
}

Word::Word(TAG tag)
    : m_Tag(tag)
{
}

TAG Word::tag(void)
{
    return m_Tag;
}

Word *Word::dereference(void)
{
    return this;
}

bool Word::compareToConst(ConstantWord *cword)
{
    return false;
}

bool Word::compareToStruct(StructureWord *sword)
{
    return false;
}

std::ostream &operator<<(std::ostream &os, const Word &word)
{
    word.print(os);
    return os;
}

ConstantWord::ConstantWord(const std::string &value)
    : Word(TAG::CONSTANT),
      m_Value(value)
{
}

void ConstantWord::print(std::ostream &os) const
{
    os << "constant -> " << m_Value;
}

Word *ConstantWord::clone(void)
{
    return new ConstantWord(m_Value);
}

bool ConstantWord::compareToConst(ConstantWord *cword)
{
    return cword->m_Value == m_Value;
}

VariableWord::VariableWord(Word **ref, bool bound)
    : Word(TAG::VARIABLE),
      m_Ref(ref),
      m_Bound(bound)
{
}

void VariableWord::print(std::ostream &os) const
{
    if (m_Bound)
    {
        os << "reference -> " << m_Ref << "[" << **m_Ref << "]";
    }
    else
    {
        os << "variable -> " << m_Ref;
    }
}

Word *VariableWord::clone(void)
{
    return new VariableWord(m_Ref, m_Bound);
}

void VariableWord::setRef(Word **ref)
{
    m_Ref = ref;
}

Word *VariableWord::dereference(void)
{
    if (m_Bound)
    {
        return (*m_Ref)->dereference();
    }

    else
    {
        return this;
    }
}

Word **VariableWord::ref(void)
{
    return m_Ref;
}

bool VariableWord::bound(void)
{
    return m_Bound;
}

ListWord::ListWord(void)
    : Word(TAG::LIST)
{
}

void ListWord::print(std::ostream &os) const
{
    os << "list";
}

Word *ListWord::clone(void)
{
    return new ListWord();
}

StructurePointerWord::StructurePointerWord(size_t heapAddress)
    : Word(TAG::S_POINTER),
      m_HeapAddress(heapAddress)
{
}

void StructurePointerWord::print(std::ostream &os) const
{
    os << "structure-pointer -> HEAP-BOT + " << m_HeapAddress;
}

Word *StructurePointerWord::clone(void)
{
    return new StructurePointerWord(m_HeapAddress);
}

StructureWord::StructureWord(const std::string &functor, size_t arity)
    : Word(TAG::STRUCTURE),
      m_Functor(functor),
      m_Arity(arity)
{
}

void StructureWord::print(std::ostream &os) const
{
    os << "structure -> " << m_Functor << "/" << m_Arity;
}

Word *StructureWord::clone(void)
{
    return new StructureWord(m_Functor, m_Arity);
}

bool StructureWord::compareToStruct(StructureWord *sword)
{
    return m_Arity == sword->m_Arity && m_Functor == sword->m_Functor;
}