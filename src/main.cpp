#include <cstdio>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

#include "compiler/Compiler.hpp"
#include "interpreter/Interpreter.hpp"
#include "preprocessor/Preprocessor.hpp"

int main(int argc, const char **argv)
{
    Renderer renderer;
    Filepath sourceCodePath = "";
    int i = 0;
    for (; i < argc; i++)
    {
        if (!strcmp(argv[i], "--file"))
        {
            if (i >= argc - 1)
            {
                std::cout << "Missing mandatory --file parameter" << std::endl;
                return 1;
            }

            sourceCodePath = argv[i + 1];
        }
        if (!strcmp(argv[i], "--step"))
        {
            renderer.setStepper(true);
        }
    }

    if (sourceCodePath.empty())
    {
        std::cout << "Missing mandatory --file parameter" << std::endl;
        return 1;
    }
    
    Preprocessor preprocessor;
    Filepath fp = preprocessor.linkLibraries(sourceCodePath, "../stdlib");

    std::ifstream ifs(fp);

    Compiler comp;
    comp.compile(ifs);

    Interpreter intp(comp.dump(), renderer);

    while (intp.run())
        ;

    std::remove(fp.c_str());

    return 0;
}