#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <box2d.h>
#include "raylib.h"

class Projectile {
public:
    Projectile(b2World* world);
    ~Projectile();

    void launch(const b2Vec2& impulse);
    void reset();
    void updatePositionToSpoon(const Vector2& spoonPos);
    void draw() const;
    bool isResting() const;
    b2Body* getBody() const { return body; }

private:
    b2Body* body;
    float radius;
    Color color;
    bool launched;
};

#endif