#include "Render.hpp"

void renderCode(std::ostream &os, const WAMCode &code, size_t pc)
{
    for (size_t i = 0; i < code.size(); i++)
    {
        if(i == pc)
        {
            os << ANSI_COLOR_B_CYAN;
        }

        code.dumpInstruction(i, os);

        os << ANSI_COLOR_DEFAULT;
    }
}

void clearScreen(std::ostream &os)
{
    os << ANSI_CLEAR_SCREEN << ANSI_CLEAR_SCREEN_BUFFER << ANSI_RETURN_CURSOR;
}