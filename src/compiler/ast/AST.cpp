#include "AST.hpp"

#include <iostream>

std::string ProgramNode::codegen( SymbolTable & st ) {
    std::string code = "";
    // Initialize the symbol table
    for ( const auto & clause : m_Clauses ) {
        auto entry = st . get ( clause -> m_Head );
        if ( ! entry )
            st . add ( clause -> m_Head, new TableEntry ( clause -> m_Head ) );
        else entry -> m_Clauses++;
    }

    for ( const auto & clause : m_Clauses ) {
        if ( ! code.empty() )
            code += "\n";
        code += clause -> codegen(st);
    }

    //Generate the "quit" label
    code += "\nquit: backtrack\n";

    return code;
}

void ProgramNode::print ( const std::string & indent ) {
    std::cout << indent << "=======[Start ProgramNode]======" << std::endl;
    std::cout << indent << "[m_Clauses] => " << std::endl;
    for ( const auto & clause : m_Clauses )
        clause -> print ( indent + " " );
    std::cout << indent << "=======[End ProgramNode]======" << std::endl;
}

UnificationNode::UnificationNode ( TermNode * x, TermNode * y )
: m_X ( x ),
  m_Y ( y )
{}

std::string UnificationNode::codegen ( SymbolTable & st ) {
    return "";
}

TermNode::TermNode ( const std::string & name )
: m_Name ( name )
{}

StructNode::StructNode ( const std::string & name, std::vector<TermNode*> args )
: TermNode( name ), 
  m_Args ( args )
{}

std::string StructNode::codegen( SymbolTable & st ) {
    std::string code = "";

    if ( m_IsGoal ) {
        for ( const auto & arg : m_Args ) {
            arg -> m_IsGoal = true;
            arg -> m_AvailableReg = m_AvailableReg;
            code += arg -> codegen(st) + "\n\t";
            m_AvailableReg = arg -> m_AvailableReg;
        }
        code += "call " + m_Name;
        return code;
    }

    code = "get-structure " + m_Name + " A" + std::to_string(m_AvailableReg++);
    for ( const auto & arg : m_Args )
        code += "\n\tunify " + arg -> m_Name;

    return code;
}

void StructNode::print ( const std::string & indent ) {
    std::cout << indent << "=======[Start StructNode]======" << std::endl;
    std::cout << indent << "[m_Name] => " << m_Name << std::endl;
    std::cout << indent << "[m_Args] => " << std::endl;
    for ( const auto & arg : m_Args )
        arg -> print ( indent + " " );
    std::cout << indent << "=======[End StructNode]======" << std::endl;
}

ListNode::ListNode ( const std::vector<TermNode*> & list, TermNode * cons )
: TermNode ( "." ),
  m_List ( list ),
  m_Cons ( cons )
{}

std::string ListNode::codegen ( SymbolTable & st ) {
    return "";
}

void ListNode::print ( const std::string & indent ) {
    std::cout << indent << "=======[Start ListNode]======" << std::endl;
    std::cout << indent << "[m_List] => " << std::endl;
    for ( const auto & term : m_List )
        term -> print ( indent + " " );
    if ( m_Cons ) {
        std::cout << indent << "[m_Cons] => " << std::endl;
        m_Cons -> print ( indent + " " );
    }
    std::cout << indent << "=======[End ListNode]======" << std::endl;
}

VarNode::VarNode ( const std::string & name )
: TermNode ( name )
{}

std::string VarNode::codegen( SymbolTable & st ) {
    return (m_IsGoal ? "put " : "get " )
        + m_Name + " A" + std::to_string(m_AvailableReg++);
}

void VarNode::print ( const std::string & indent ) {
    std::cout << indent << "=======[Start VarNode]======" << std::endl;
    std::cout << indent << "[m_Name] => " << m_Name << std::endl;
    std::cout << indent << "=======[End VarNode]======" << std::endl;
}

std::string ConstNode::codegen( SymbolTable & st ) {
    return "";
}

void ConstNode::print ( const std::string & indent ) {
    std::cout << indent << "=======[Start ConstNode]======" << std::endl;

    std::cout << indent << "=======[End ConstNode]======" << std::endl;
}

ClauseNode::ClauseNode ( const std::string       & head, 
                         std::vector<TermNode *>   args, 
                         std::vector<GoalNode *>   body )
: m_Head ( head ),
  m_Args ( args ),
  m_Body ( body )
{}

std::string ClauseNode::codegen ( SymbolTable & st ) {
    std::string code = "";
    TableEntry * entry = st . get ( m_Head );
    // Generate the initial mark instruction for first clause of the predicate name
    if ( ! entry -> m_Generated )
        code += m_Head + ":\tmark\n";
    else
        code += m_Head + std::to_string(entry -> m_Generated) + ":";
    
    ++entry -> m_Generated;
    std::string retryLabel = entry -> 
        m_Generated == entry -> m_Clauses ? "quit" : m_Head + std::to_string(entry -> m_Generated);
    code += "\tretry-me-else " + retryLabel + "\n";

    size_t currentArgumentRegister = 1;
    for ( size_t i = 0; i < m_Args . size(); i++ ) {
        m_Args[i] -> m_IsGoal = false;
        m_Args[i] -> m_AvailableReg = currentArgumentRegister;
        //Load the arguments into argument reigsters
        code += "\t" + m_Args[i] -> codegen(st) + "\n";
        currentArgumentRegister = m_Args[i] -> m_AvailableReg;
    }

    //All get instructions were carried out
    currentArgumentRegister = 1;

    for ( size_t i = 0; i < m_Body . size(); i++ ) {
        m_Body[i] -> m_AvailableReg = currentArgumentRegister;
        code += "\t" + m_Body[i] -> codegen(st) + "\n";
        currentArgumentRegister = m_Body[i] -> m_AvailableReg;
    }

    return code + "\treturn\n";
}

void ClauseNode::print ( const std::string & indent ) {
    std::cout << indent << "=======[Start ClauseNode]======" << std::endl;
    std::cout << indent << "[m_Head] => " << m_Head << std::endl;
    std::cout << indent << "[m_Args] => " << std::endl;
    for ( const auto & arg : m_Args )
        arg -> print ( indent + " " );
    std::cout << indent << "[m_Body] => " << std::endl;
    for ( const auto & goal : m_Body )
        goal -> print ( indent + " " );
    std::cout << indent << "=======[End ClauseNode]======" << std::endl;
}