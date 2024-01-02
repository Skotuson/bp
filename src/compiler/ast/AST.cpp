#include "AST.hpp"

std::string ProgramNode::codegen( void ) {
    return "";
}

StructNode::StructNode ( std::vector<TermNode*> args )
: m_Args ( args )
{}

std::string StructNode::codegen( void ) {
    return "";
}

std::string VarNode::codegen( void ) {
    return "";
}

std::string ConstNode::codegen( void ) {
    return "";
}