#pragma once

const std::string ANSI_SHOW_CURSOR     = "\033[?25h",
                  ANSI_HIDE_CURSOR     = "\033[?25l",
                  ANSI_RETURN_CURSOR   = "\033[H",
                  ANSI_CLEAR_LINE      = "\033[2K",
                  ANSI_CLEAR_SCREEN    = "\033[2J",
                  ANSI_COLOR_DEFAULT   = "\033[0m",
                  ANSI_COLOR_BLACK     = "\033[0;30m",
                  ANSI_COLOR_RED       = "\033[0;31m",
                  ANSI_COLOR_GREEN     = "\033[0;32m",
                  ANSI_COLOR_YELLOW    = "\033[0;33m",
                  ANSI_COLOR_BLUE      = "\033[0;34m",
                  ANSI_COLOR_MAGENTA   = "\033[0;35m",
                  ANSI_COLOR_CYAN      = "\033[0;36m",
                  ANSI_COLOR_WHITE     = "\033[0;37m",
                  ANSI_COLOR_B_GREEN   = "\033[0;92m",
                  ANSI_COLOR_B_YELLOW  = "\033[0;93m",
                  ANSI_COLOR_B_BLUE    = "\033[0;94m",
                  ANSI_COLOR_B_MAGENTA = "\033[0;95m",
                  ANSI_COLOR_B_CYAN    = "\033[0;96m";