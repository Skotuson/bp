#define DOCTEST_CONFIG_IMPLEMENT
#include "../test/doctest.h"

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

    doctest::Context context;

    // defaults
    context.addFilter("test-case-exclude", "*math*"); // exclude test cases with "math" in their name
    context.setOption("abort-after", 5);              // stop test execution after 5 failed assertions
    context.setOption("order-by", "name");            // sort the test cases by their name

    context.applyCommandLine(argc, argv);

    // overrides
    context.setOption("no-breaks", true); // don't break in the debugger when assertions fail

    int res = context.run(); // run

    if (context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
        return res;           // propagate the result of the tests

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

    Preprocessor preprocessor;
    // TODO: add directory iterator
    Filepath fp = preprocessor.linkLibraries(sourceCodePath, "stdlib");

    std::ifstream ifs(fp);

    if (!ifs)
    {
        std::cout << "File couldn't be opened" << std::endl;
        return 0;
    }

    Compiler comp(ifs);
    comp.compile();

    Interpreter intp(comp.dump(), renderer);

    while (intp.run())
        ;

    std::remove(fp.c_str());

    return res;
}