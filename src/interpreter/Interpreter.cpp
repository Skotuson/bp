#include "Interpreter.hpp"

Interpreter::Interpreter(std::istream &is)
{}

Instruction Interpreter::fetch(void) {
    return program[programCounter];
}

void Interpreter::decode(void) {

}

void Interpreter::execute(void) {

}