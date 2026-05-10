#ifndef PHYSICS_BOX_H
#define PHYSICS_BOX_H

#include <box2d.h>
#include "raylib.h"

class PhysicsBox {
public:
    PhysicsBox(b2World* world, float x, float y, float width, float height, Color color);
    ~PhysicsBox();

    void draw() const;
    void reset();

private:
    b2Body* body;
    float width, height;
    Color color;
    b2Vec2 initialPos;
    float initialAngle;
};

#endif