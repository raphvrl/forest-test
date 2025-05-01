#include <iostream>
#include "core/game/game.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    try {
        core::Game game;
        game.run();
    } catch (const std::exception &e) {
        core::Logger::error(e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}