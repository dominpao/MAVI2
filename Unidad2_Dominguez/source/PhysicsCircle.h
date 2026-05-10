#ifndef PHYSICS_CIRCLE_H
#define PHYSICS_CIRCLE_H

#include <box2d.h>
#include "raylib.h"

class PhysicsCircle {
public:
    PhysicsCircle(b2World* world, float x, float y, float radius, Color color);
    ~PhysicsCircle();

    void draw() const;
    void reset();

private:
    b2Body* body;
    float radius;
    Color color;
    b2Vec2 initialPos;
    float initialAngle;
};

#endif