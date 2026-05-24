#ifndef BOX_PLAYER_H
#define BOX_PLAYER_H

#include <box2d.h>
#include <raylib.h>

class BoxPlayer {
private:
    b2Body* body;
    Color color;
    b2Vec2 startPosition;

public:
    BoxPlayer(b2World* world, float x, float y, Color displayColor);

    void ApplyHorizontalForce(float forceX);
    void LimitVelocity(float maxVel);
    void Jump(float impulseY);
    void HandleVacuumFall();
    void Reset();

    b2Body* GetBody() const { return body; }
    b2Vec2 GetPosition() const { return body->GetPosition(); }
    void Draw(float ppm, Vector2(*toScreen)(b2Vec2)) const;
    bool IsGrounded() const;
};

#endif
