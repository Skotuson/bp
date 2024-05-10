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
    bool dumpOnly = false;
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
        if (!strcmp(argv[i], "--dump"))
        {
            dumpOnly = true;
        }
    }

    if (sourceCodePath.empty())
    {
        std::cout << "Missing mandatory --file parameter" << std::endl;
        return 1;
    }

    Preprocessor preprocessor;
    std::istringstream iss(
        "__id(A, A).\n"
        "__zero(0).");
    Filepath fp = preprocessor.linkLibrary(sourceCodePath, iss);

    std::ifstream ifs(fp);

    Compiler comp;
    comp.compile(ifs);

    Interpreter intp(comp.dump(), renderer);
    intp.setDumpOnly(dumpOnly);

    while (intp.run())
        ;

    std::remove(fp.c_str());

    return 0;
}