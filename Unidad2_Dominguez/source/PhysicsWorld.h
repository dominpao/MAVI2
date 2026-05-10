#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H

#include <box2d.h>

class PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();

    void step();
    b2World* getWorld() { return &world; }

private:
    b2World world;
    void createGround();
    void createWalls();
};

#endif