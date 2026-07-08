#include <raylib.h>
#include "Game.h"

int main() {
    InitWindow(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, "Arquero Game - State Machine");
    SetTargetFPS(60);

    Game game;

    while (!game.shouldClose()) {
        game.update();
        game.draw();
    }

    CloseWindow();
    return 0;
}