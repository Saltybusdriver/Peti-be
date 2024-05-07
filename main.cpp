#include "game.h"
#include <iostream> 
int main(int argc, char* args[]) {
    Game game;
    if (!game.init()) {
        std::cerr << "Failed to initialize game.\n";
        return -1;
    }
    game.run();
    return 0;
}