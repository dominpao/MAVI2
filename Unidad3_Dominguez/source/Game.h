#ifndef GAME_H
#define GAME_H

#include <box2d.h>
#include <raylib.h>
#include <string> 
#include "BoxPlayer.h"
#include "SuspensionBridge.h"
#include "Elevator.h"

class Game {
private:
    b2World* world;
    b2Body* ground;
    b2Body* startPlatform;

    BoxPlayer* cajaRoja;
    BoxPlayer* cajaAzul;
    SuspensionBridge* bridge;
    Elevator* elevator;

    bool azulActiva;

    void HandleInput();
    void ApplyScreenLimits(BoxPlayer* player);

public:
    Game();
    ~Game();
    void Run();

    static Vector2 ToScreen(b2Vec2 worldPos);
};

#endif
