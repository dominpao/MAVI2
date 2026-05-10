#ifndef CATAPULT_H
#define CATAPULT_H

#include "raylib.h"
#include <box2d.h>

class Catapult {
public:
    Catapult();
    ~Catapult();

    void update(float dt, bool canCharge, bool& launchTrigger, b2Vec2& outImpulse, Vector2& outSpoonPos);
    void draw() const;
    void reset();
    Vector2 getSpoonPosition() const;
    bool isCharging() const { return charging; }
    float getChargePercent() const;   

private:
    Vector2 basePos;
    Vector2 pivot;
    float armLength;
    float currentAngleDeg;
    bool charging;
    float chargeTime;

    void releaseCharge(b2Vec2& outImpulse);
};

#endif