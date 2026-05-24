#include "Elevator.h"
#include <cmath>

Elevator::Elevator(b2World* world, b2Body* ground, float x, float y)
    : surfaceY(y), active(false), complete(false) {

    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(x, y);
    body = world->CreateBody(&def);

    b2PolygonShape shape;
    shape.SetAsBox(1.6f, 0.3f);

    b2FixtureDef fix;
    fix.shape = &shape;
    fix.density = 5.0f;
    fix.friction = 1.0f;
    body->CreateFixture(&fix);

    b2PrismaticJointDef prismaticDef;
    b2Vec2 worldAxis(0.0f, -1.0f);
    prismaticDef.Initialize(ground, body, body->GetPosition(), worldAxis);
    prismaticDef.enableLimit = true;
    prismaticDef.lowerTranslation = 0.0f;
    prismaticDef.upperTranslation = y - 1.8f;
    prismaticDef.enableMotor = true;
    prismaticDef.maxMotorForce = 5000.0f;
    prismaticDef.motorSpeed = 0.0f;

    joint = static_cast<b2PrismaticJoint*>(world->CreateJoint(&prismaticDef));
}

void Elevator::UpdateSensors(b2Vec2 posRoja, b2Vec2 posAzul) {
    if (complete) return;

    float elevatorTop = body->GetPosition().y + 0.3f;

    float dxRoja = std::fabs(posRoja.x - body->GetPosition().x);
    bool rojaEncima = (dxRoja < 1.5f && (posRoja.y - 0.6f) <= elevatorTop + 0.2f && (posRoja.y - 0.6f) >= elevatorTop - 0.2f);

    float dxAzul = std::fabs(posAzul.x - body->GetPosition().x);
    bool azulEncima = (dxAzul < 1.5f && (posAzul.y - 0.6f) <= elevatorTop + 0.2f && (posAzul.y - 0.6f) >= elevatorTop - 0.2f);

    if (!active && rojaEncima && azulEncima) {
        active = true;
    }
}

void Elevator::UpdatePhysics() {
    if (active && !complete) {
        joint->SetMotorSpeed(2.2f);
        if (body->GetPosition().y <= 1.8f) {
            joint->SetMotorSpeed(0.0f);
            complete = true;
            active = false;
        }
    }
    else {
        joint->SetMotorSpeed(0.0f);
    }
}

void Elevator::Reset(float x, float y) {
    joint->SetMotorSpeed(0.0f);
    body->SetTransform(b2Vec2(x, y), 0);
    complete = false;
    active = false;
}

void Elevator::Draw(float ppm, Vector2(*toScreen)(b2Vec2)) const {
    Vector2 elevPos = toScreen(body->GetPosition());
    float groundY = toScreen(b2Vec2(0.0f, 0.5f)).y;
    Vector2 railTop = toScreen(b2Vec2(22.2f, surfaceY + 0.5f));

    DrawRectangle(static_cast<int>(railTop.x - 6), static_cast<int>(railTop.y), 12, static_cast<int>(groundY - railTop.y), DARKGRAY);
    for (int y = static_cast<int>(railTop.y); y < groundY; y += 15) {
        DrawRectangle(static_cast<int>(railTop.x - 10), y, 4, 5, BLACK);
    }

    DrawRectangle(static_cast<int>(railTop.x - 15), static_cast<int>(groundY - 120), 30, 120, GRAY);
    DrawRectangle(static_cast<int>(railTop.x - 8), static_cast<int>(elevPos.y), 16, static_cast<int>(groundY - 120 - elevPos.y), LIGHTGRAY);

    DrawRectangle(static_cast<int>(elevPos.x - 1.6f * ppm), static_cast<int>(elevPos.y - 0.3f * ppm), static_cast<int>(3.2f * ppm), static_cast<int>(0.6f * ppm), BLUE);
    DrawRectangleLines(static_cast<int>(elevPos.x - 1.6f * ppm), static_cast<int>(elevPos.y - 0.3f * ppm), static_cast<int>(3.2f * ppm), static_cast<int>(0.6f * ppm), DARKBLUE);

    Color motorLight = GREEN;
    if (active && !complete) motorLight = ORANGE;
    else if (complete) motorLight = RED;
    DrawCircle(static_cast<int>(elevPos.x), static_cast<int>(elevPos.y), 5, motorLight);
}