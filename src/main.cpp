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
                std::cout << "Missing --file parameter" << std::endl;
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

    Preprocessor preprocessor;
    Filepath fp = preprocessor.linkStandard(sourceCodePath);

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