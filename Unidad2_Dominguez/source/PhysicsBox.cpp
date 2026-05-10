#include "PhysicsBox.h"

PhysicsBox::PhysicsBox(b2World* world, float x, float y, float w, float h, Color col)
    : width(w), height(h), color(col), initialPos(x, y), initialAngle(0.0f) {
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(x, y);
    body = world->CreateBody(&def);

    b2PolygonShape shape;
    shape.SetAsBox(width / 2.0f, height / 2.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.4f;
    fixtureDef.restitution = 0.2f;
    body->CreateFixture(&fixtureDef);
}

PhysicsBox::~PhysicsBox() {}

void PhysicsBox::draw() const {
    b2Vec2 pos = body->GetPosition();
    float angle = body->GetAngle() * RAD2DEG;
    Rectangle rect = { pos.x - width / 2.0f, pos.y - height / 2.0f, width, height };
    Vector2 origin = { width / 2.0f, height / 2.0f };
    DrawRectanglePro(rect, origin, angle, color);
    DrawRectangleLinesEx(rect, 2, DARKBLUE);
}

void PhysicsBox::reset() {
    body->SetTransform(initialPos, initialAngle);
    body->SetLinearVelocity(b2Vec2_zero);
    body->SetAngularVelocity(0.0f);
    body->SetAwake(true);
}