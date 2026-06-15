#include "Game.h"
#include "Constants.h"
#include <raylib.h>

int main() {
    // Inicializar la ventana ANTES de cualquier otra cosa de Raylib
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Capy y Amigo - Desafio Final");
    SetTargetFPS(60);

    // Inicializar el juego
    Game game;
    game.run();

    // Cerrar la ventana al final
    CloseWindow();
    return 0;
}