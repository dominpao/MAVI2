#include "Catapult.h"
#include "Constants.h"
#include <cmath>

#define MAX_CHARGE_TIME 2.0f

Catapult::Catapult()
    : armLength(ARM_LENGTH),
    currentAngleDeg(ARM_REST_ANGLE_DEG),
    charging(false),
    chargeTime(0.0f) {
    basePos.x = CATAPULT_BASE_X;
    basePos.y = CATAPULT_BASE_Y;
    pivot.x = basePos.x + 15.0f;
    pivot.y = basePos.y - 35.0f;
}

Catapult::~Catapult() {}

Vector2 Catapult::getSpoonPosition() const {
    float rad = currentAngleDeg * DEG2RAD;
    Vector2 spoon;
    spoon.x = pivot.x + armLength * cos(rad);
    spoon.y = pivot.y + armLength * sin(rad);
    return spoon;
}

float Catapult::getChargePercent() const {
    return chargeTime / MAX_CHARGE_TIME;
}

void Catapult::update(float dt, bool canCharge, bool& launchTrigger, b2Vec2& outImpulse, Vector2& outSpoonPos) {
    launchTrigger = false;
    outSpoonPos = getSpoonPosition();

    if (canCharge && !charging && IsKeyPressed(KEY_SPACE)) {
        charging = true;
        chargeTime = 0.0f;
    }

    if (charging && IsKeyDown(KEY_SPACE)) {
        chargeTime += dt;
        if (chargeTime > MAX_CHARGE_TIME) chargeTime = MAX_CHARGE_TIME;
        float t = chargeTime / MAX_CHARGE_TIME;
        currentAngleDeg = ARM_REST_ANGLE_DEG - t * (ARM_REST_ANGLE_DEG - ARM_MAX_TENSION_DEG);
    }

    if (charging && IsKeyReleased(KEY_SPACE)) {
        releaseCharge(outImpulse);
        charging = false;
        currentAngleDeg = ARM_REST_ANGLE_DEG;
        launchTrigger = true;
    }

    if (!charging && currentAngleDeg != ARM_REST_ANGLE_DEG) {
        currentAngleDeg = ARM_REST_ANGLE_DEG;
    }
}

void Catapult::releaseCharge(b2Vec2& outImpulse) {
    float t = chargeTime / MAX_CHARGE_TIME;
    float magnitude = BASE_IMPULSE + t * EXTRA_IMPULSE;
    float angleRad = LAUNCH_ANGLE_DEG * DEG2RAD;
    outImpulse.Set(magnitude * cos(angleRad), -magnitude * sin(angleRad));
}

void Catapult::reset() {
    charging = false;
    chargeTime = 0.0f;
    currentAngleDeg = ARM_REST_ANGLE_DEG;
}

void Catapult::draw() const {
    DrawRectangle(static_cast<int>(basePos.x - 30), static_cast<int>(basePos.y - 10), 80, 20, BROWN);
    DrawRectangle(static_cast<int>(basePos.x - 20), static_cast<int>(basePos.y - 20), 60, 10, DARKBROWN);
    DrawCircle(static_cast<int>(basePos.x - 15), static_cast<int>(basePos.y + 5), 8, BLACK);
    DrawCircle(static_cast<int>(basePos.x + 25), static_cast<int>(basePos.y + 5), 8, BLACK);
    DrawRectangle(static_cast<int>(basePos.x + 10), static_cast<int>(basePos.y - 40), 10, 40, BEIGE);
    Vector2 spoon = getSpoonPosition();
    DrawLineEx(pivot, spoon, 8, BROWN);
    DrawCircle(static_cast<int>(spoon.x), static_cast<int>(spoon.y), 10, DARKBROWN);
}