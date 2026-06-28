#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "Config.h"
#include "Types.h"
#include "Level.h"

class Game {
public:
    Game();
    ~Game();

    void Run();

private:
    Level* level;
    GameState gameState;
    float stateTimer;

    Texture2D capyTex;
    Texture2D platTex;

    void Init();
    void Shutdown();
    void HandleInput();
    void Update();
    void Draw();
    void DrawPlatforms();
    void DrawPlayer();
    void DrawFlag(float x, float y, bool isUnlocked);
    void DrawUI();
};

#endif
