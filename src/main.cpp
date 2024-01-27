#include <cassert>
#include <iostream>
#include <fstream>

#include "compiler/Compiler.hpp"

int main ( int argc, const char ** argv ) {
    std::ostream & os = std::cout;    

    if ( argc != 2 ) {
        std::cout << "Usage: [filename]" << std::endl;
        return 0;
    }

    std::ifstream ifs ( argv[1] );

    if ( ! ifs ) {
        std::cout << "File couldn't be opened" << std::endl;
        return 0;
    } 

    Compiler comp(ifs, os);
    comp.compile();
    os << std::endl;
    return 0;
}