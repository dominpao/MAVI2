#include "Game.h"
#include <raylib.h>
#include <string>
#include <sstream>

Game::Game() : gameWon(false), blueActive(true) {
    playerRed = new Player(worldManager.getWorld(), 10.5f, 2.2f, RED);
    playerBlue = new Player(worldManager.getWorld(), 13.5f, 2.2f, BLUE);
    playerRed->setActive(false);
    playerBlue->setActive(true);
}

Game::~Game() {
    delete playerRed;
    delete playerBlue;
}

void Game::handleInput() {
    if (IsKeyPressed(KEY_R)) resetGame();

    if (!gameWon) {
        Player* current = blueActive ? playerBlue : playerRed;

        if (IsKeyDown(KEY_RIGHT)) current->moveRight(80.0f);
        if (IsKeyDown(KEY_LEFT)) current->moveLeft(80.0f);
        if (IsKeyPressed(KEY_UP)) current->jump(12.0f);

        if (IsKeyPressed(KEY_SPACE)) {
            blueActive = !blueActive;
            playerRed->setActive(!blueActive);
            playerBlue->setActive(blueActive);
        }
    }
}

void Game::updateGameLogic() {
    if (!gameWon) {
        worldManager.updateMovingPlatform();
        worldManager.constrainPlayers(playerRed->getBody(), playerBlue->getBody());
        playerRed->updateGroundStatus();
        playerBlue->updateGroundStatus();
        checkWinCondition();
    }
    worldManager.step(1.0f / 60.0f, 8, 3);
}

// Verifica si ambos personajes llegaron a la plataforma dorada
void Game::checkWinCondition() {
    bool redInMeta = playerRed->isInMeta(24.5f, 8.5f, 10.0f);
    bool blueInMeta = playerBlue->isInMeta(24.5f, 8.5f, 10.0f);
    if (redInMeta && blueInMeta) gameWon = true;
}

void Game::resetGame() {
    gameWon = false;
    blueActive = true;
    playerRed->reset(10.5f, 2.2f);
    playerBlue->reset(13.5f, 2.2f);
    playerRed->setActive(false);
    playerBlue->setActive(true);
}

void Game::run() {
    while (!WindowShouldClose()) {
        handleInput();
        updateGameLogic();

        renderer.beginDrawing();
        renderer.clearBackground(SKYBLUE);

        // UI
        renderer.drawUI("ESPACIO: cambiar | R: reiniciar | FLECHAS: mover | META: plataforma dorada", 20, 20, 20, DARKGRAY);
        renderer.drawUI(blueActive ? "Controlando: CAPY" : "Controlando: AMIGO", 20, 50, 20, blueActive ? BLUE : RED);
        if (!renderer.isTextureLoaded()) renderer.drawUI("Modo cubos", 20, 110, 15, ORANGE);
        if (gameWon) renderer.drawVictoryScreen();

        // Plataformas
        renderer.drawGround();
        renderer.drawPlatform(worldManager.getCentralPlatform()->GetPosition(), 3.5f, 0.3f, LIGHTGRAY);
        renderer.drawPlatform(b2Vec2(2.0f, 8.0f), 0.25f, 8.0f, DARKGRAY);
        if (worldManager.getBrazo()) renderer.drawPlatform(worldManager.getBrazo()->GetPosition(), 5.0f, 0.15f, BROWN);
        if (worldManager.getPendulumPlatform()) renderer.drawPlatform(worldManager.getPendulumPlatform()->GetPosition(), 1.5f, 0.15f, ORANGE);
        renderer.drawPlatform(worldManager.getPlatMov()->GetPosition(), 2.5f, 0.15f, PURPLE);
        renderer.drawPlatform(worldManager.getMetaPlat()->GetPosition(), 2.0f, 0.3f, GOLD);

        // Linea del pendulo
        if (worldManager.getPendulumPlatform())
            renderer.drawLine(b2Vec2(7.0f, 12.0f), worldManager.getPendulumPlatform()->GetPosition(), 3.0f, BLACK);

        playerRed->draw(&renderer);
        playerBlue->draw(&renderer);

        renderer.endDrawing();
    }
}