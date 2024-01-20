#include "AST.hpp"

#include <iostream>

std::string ProgramNode::codegen( SymbolTable & st ) {
    std::string code = "";
    for ( const auto & clause : m_Clauses ) {
        if ( ! code.empty() )
            code += "\n";
        code += clause -> codegen(st);
    }
    return code;
}

void ProgramNode::print ( const std::string & indent ) {
    std::cout << indent << "=======[Start ProgramNode]======" << std::endl;
    std::cout << indent << "[m_Clauses] => " << std::endl;
    for ( const auto & clause : m_Clauses )
        clause -> print ( indent + " " );
    std::cout << indent << "=======[End ProgramNode]======" << std::endl;
}

StructNode::StructNode ( const std::string & name, std::vector<TermNode*> args )
: m_Name ( name ), 
  m_Args ( args )
{}

std::string StructNode::codegen( SymbolTable & st ) {
    return "get-structure " + m_Name;
}

void StructNode::print ( const std::string & indent ) {
    std::cout << indent << "=======[Start StructNode]======" << std::endl;
    std::cout << indent << "[m_Name] => " << m_Name << std::endl;
    std::cout << indent << "[m_Args] => " << std::endl;
    for ( const auto & arg : m_Args )
        arg -> print ( indent + " " );
    std::cout << indent << "=======[End StructNode]======" << std::endl;
}

VarNode::VarNode ( const std::string & name )
: m_Name ( name )
{}

std::string VarNode::codegen( SymbolTable & st ) {
    return "get " + m_Name;
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
    if ( ! entry ) {
        st . add ( m_Head, new TableEntry ( m_Head ) );
        // Generate the initial mark instruction for first clause of the predicate name
        code += m_Head + ":\tmark\n";
        //TODO: generate the retry-me-else instruction
    } else {
        entry -> m_Clauses++;
        code += m_Head + std::to_string(entry -> m_Clauses) + ":\t\n";
    }
    
    for ( size_t i = 0; i < m_Args . size(); i++ )
        //Load the arguments into argument reigsters
        code += "\t" + m_Args[i] -> codegen(st) + " A" + std::to_string(i + 1) + "\n";

    return code + "\n\treturn";
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