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

ClauseNode::ClauseNode ( std::vector<TermNode *> args, std::vector<GoalNode *> body )
: m_Args ( args ),
  m_Body ( body )
{}

std::string ClauseNode::codegen ( void ) {
    return "";
}