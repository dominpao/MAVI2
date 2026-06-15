#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "WorldManager.h"
#include "Renderer.h"

class Game {
private:
    WorldManager worldManager;
    Renderer renderer;
    Player* playerRed;
    Player* playerBlue;
    bool gameWon;
    bool blueActive;

    void handleInput();
    void updateGameLogic();
    void checkWinCondition();
    void resetGame();

public:
    Game();
    ~Game();

    void run();
};

#endif