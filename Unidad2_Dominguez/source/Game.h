#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <vector>
#include "PhysicsWorld.h"
#include "Projectile.h"
#include "Catapult.h"
#include "PhysicsBox.h"
#include "PhysicsCircle.h"

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    PhysicsWorld physicsWorld;
    Projectile projectile;
    Catapult catapult;
    std::vector<PhysicsBox> boxes;
    std::vector<PhysicsCircle> circles;

    void processInput();
    void update();
    void draw() const;
    void resetAll();
    void createObjects();
};

#endif