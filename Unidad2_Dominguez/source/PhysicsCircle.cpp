#include "PhysicsCircle.h"

PhysicsCircle::PhysicsCircle(b2World* world, float x, float y, float r, Color col)
    : radius(r), color(col), initialPos(x, y), initialAngle(0.0f) {
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(x, y);
    body = world->CreateBody(&def);

    b2CircleShape shape;
    shape.m_radius = radius;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.6f;
    body->CreateFixture(&fixtureDef);
}

PhysicsCircle::~PhysicsCircle() {}

void PhysicsCircle::draw() const {
    b2Vec2 pos = body->GetPosition();
    DrawCircleV({ pos.x, pos.y }, radius, color);
    DrawCircleLines(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(radius), BROWN);
}

void PhysicsCircle::reset() {
    body->SetTransform(initialPos, initialAngle);
    body->SetLinearVelocity(b2Vec2_zero);
    body->SetAngularVelocity(0.0f);
    body->SetAwake(true);
}