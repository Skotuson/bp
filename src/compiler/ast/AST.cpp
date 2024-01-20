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

    return code;
}

void ProgramNode::print ( const std::string & indent ) {
    std::cout << indent << "=======[Start ProgramNode]======" << std::endl;
    std::cout << indent << "[m_Clauses] => " << std::endl;
    for ( const auto & clause : m_Clauses )
        clause -> print ( indent + " " );
    std::cout << indent << "=======[End ProgramNode]======" << std::endl;
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
    code += "get-structure " + m_Name;

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

VarNode::VarNode ( const std::string & name )
: TermNode ( name )
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
    // Generate the initial mark instruction for first clause of the predicate name
    if ( ! entry -> m_Generated )
        code += m_Head + ":\tmark\n";
    else
        code += m_Head + std::to_string(entry -> m_Generated) + ":";
    
    ++entry -> m_Generated;
    std::string retryLabel = entry -> 
        m_Generated == entry -> m_Clauses ? "quit" : m_Head + std::to_string(entry -> m_Generated);
    code += "\tretry-me-else " + retryLabel + "\n";

    for ( size_t i = 0; i < m_Args . size(); i++ )
        //Load the arguments into argument reigsters
        code += "\t" + m_Args[i] -> codegen(st) + " A" + std::to_string(i + 1) + "\n";

    for ( size_t i = 0; i < m_Body . size(); i++ )
        code += "\t";

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