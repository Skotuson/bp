#include "Renderer.hpp"

void Renderer::renderCode(std::ostream &os, const WAMCode &code, size_t pc)
{
    for (size_t i = 0; i < code.size(); i++)
    {
        if (i == pc)
        {
            os << ANSI_COLOR_B_CYAN;
        }

        code.dumpInstruction(i, os);

        os << ANSI_COLOR_DEFAULT;
    }
}

void Renderer::clearScreen(std::ostream &os)
{
    os << ANSI_CLEAR_SCREEN << ANSI_CLEAR_SCREEN_BUFFER << ANSI_RETURN_CURSOR;
}

std::string Renderer::printInColor(const std::string &str, const std::string &ansi_color)
{
    return ansi_color + str + ANSI_COLOR_DEFAULT;
}

bool Renderer::step(void)
{
    return m_Step;
}

void Renderer::setStepper(bool step)
{
    m_Step = step;
}