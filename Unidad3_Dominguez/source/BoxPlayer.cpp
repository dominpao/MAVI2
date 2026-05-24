#include "BoxPlayer.h"
#include <cmath>

BoxPlayer::BoxPlayer(b2World* world, float x, float y, Color displayColor)
    : color(displayColor), startPosition(x, y) {

    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(x, y);
    def.fixedRotation = true;
    body = world->CreateBody(&def);

    b2PolygonShape boxShape;
    boxShape.SetAsBox(0.5f, 0.6f);

    b2FixtureDef fix;
    fix.shape = &boxShape;
    fix.density = 1.3f;
    fix.friction = 0.9f;
    body->CreateFixture(&fix);
}

void BoxPlayer::ApplyHorizontalForce(float forceX) {
    body->ApplyForceToCenter(b2Vec2(forceX, 0), true);
}

void BoxPlayer::LimitVelocity(float maxVel) {
    b2Vec2 vel = body->GetLinearVelocity();
    if (vel.x > maxVel) vel.x = maxVel;
    if (vel.x < -maxVel) vel.x = -maxVel;
    if (!IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)) vel.x *= 0.92f;
    body->SetLinearVelocity(vel);
}

void BoxPlayer::Jump(float impulseY) {
    b2Vec2 vel = body->GetLinearVelocity();
    body->SetLinearVelocity(b2Vec2(vel.x, 0));
    body->ApplyLinearImpulseToCenter(b2Vec2(0, impulseY), true);
}

void BoxPlayer::HandleVacuumFall() {
    if (body->GetPosition().y < 1.2f) {
        Reset();
    }
}

void BoxPlayer::Reset() {
    body->SetTransform(startPosition, 0);
    body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    body->SetAngularVelocity(0.0f);
}

bool BoxPlayer::IsGrounded() const {
    for (b2ContactEdge* ce = body->GetContactList(); ce != nullptr; ce = ce->next) {
        b2Contact* contact = ce->contact;
        if (contact && contact->IsTouching()) {
            b2WorldManifold worldManifold;
            contact->GetWorldManifold(&worldManifold);
            int pointCount = contact->GetManifold()->pointCount;
            for (int i = 0; i < pointCount; ++i) {
                if (worldManifold.points[i].y < body->GetPosition().y - 0.4f) {
                    return true;
                }
            }
        }
    }
    return false;
}

void BoxPlayer::Draw(float ppm, Vector2(*toScreen)(b2Vec2)) const {
    Vector2 pos = toScreen(body->GetPosition());
    DrawRectangle(static_cast<int>(pos.x - 0.5f * ppm), static_cast<int>(pos.y - 0.6f * ppm),
        static_cast<int>(1.0f * ppm), static_cast<int>(1.2f * ppm), color);
}