#include <iostream>

#include "core/game.hpp"

int main()
{
    try {
        core::Game game;
        game.run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}