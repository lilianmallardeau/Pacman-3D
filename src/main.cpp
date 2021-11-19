#include <iostream>
#include "game/Game.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Provide the map to load as command line argument." << std::endl;
        return EXIT_FAILURE;
    }

    Game game(argv[1]);
    if (!game.init())
        return EXIT_FAILURE;

    game.loop();

    return EXIT_SUCCESS;
}