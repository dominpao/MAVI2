#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <box2d.h>
#include <raylib.h>

class Elevator {
private:
    b2Body* body;
    b2PrismaticJoint* joint;
    float surfaceY;
    bool active;
    bool complete;

public:
    Elevator(b2World* world, b2Body* ground, float x, float y);

    void UpdateSensors(b2Vec2 posRoja, b2Vec2 posAzul);
    void UpdatePhysics();
    void Reset(float x, float y);

    bool IsActive() const { return active; }
    bool IsComplete() const { return complete; }
    b2Body* GetBody() const { return body; }

    void Draw(float ppm, Vector2(*toScreen)(b2Vec2)) const;
};

#endif
