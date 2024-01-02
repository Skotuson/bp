#include "AST.hpp"

#include <iostream>

std::string ProgramNode::codegen( void ) {
    return "";
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

std::string StructNode::codegen( void ) {
    return "";
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

std::string VarNode::codegen( void ) {
    return "";
}

void VarNode::print ( const std::string & indent ) {
    std::cout << indent << "=======[Start VarNode]======" << std::endl;
    std::cout << indent << "[m_Name] => " << m_Name << std::endl;
    std::cout << indent << "=======[End VarNode]======" << std::endl;
}

std::string ConstNode::codegen( void ) {
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

std::string ClauseNode::codegen ( void ) {
    return "";
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