#include "Projectile.h"
#include "Constants.h"
#include <cmath>

Projectile::Projectile(b2World* world) : radius(PROJECTILE_RADIUS), color(RED), launched(false) {
    b2BodyDef projDef;
    projDef.type = b2_dynamicBody;
    projDef.position.Set(200.0f, GROUND_Y - radius);
    body = world->CreateBody(&projDef);

    b2CircleShape shape;
    shape.m_radius = radius;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = PROJECTILE_DENSITY;
    fixtureDef.friction = PROJECTILE_FRICTION;
    fixtureDef.restitution = PROJECTILE_RESTITUTION;
    body->CreateFixture(&fixtureDef);

    body->SetGravityScale(0.0f);
    body->SetAwake(false);
}

Projectile::~Projectile() {}

void Projectile::launch(const b2Vec2& impulse) {
    body->SetGravityScale(1.0f);
    body->SetLinearVelocity(b2Vec2_zero);
    body->SetAngularVelocity(0.0f);
    body->SetAwake(true);
    body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);
    launched = true;
}

void Projectile::reset() {
    launched = false;
    body->SetGravityScale(0.0f);
    body->SetLinearVelocity(b2Vec2_zero);
    body->SetAngularVelocity(0.0f);
    body->SetAwake(false);
}

void Projectile::updatePositionToSpoon(const Vector2& spoonPos) {
    if (!launched) {
        body->SetTransform(b2Vec2(spoonPos.x, spoonPos.y), 0.0f);
        body->SetLinearVelocity(b2Vec2_zero);
        body->SetAngularVelocity(0.0f);
        body->SetAwake(true);
    }
}

void Projectile::draw() const {
    b2Vec2 pos = body->GetPosition();
    DrawCircleV({ pos.x, pos.y }, radius, color);
    DrawCircleLines(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(radius), MAROON);
}

bool Projectile::isResting() const {
    if (launched) return false;
    b2Vec2 vel = body->GetLinearVelocity();
    float speed = std::sqrt(vel.x * vel.x + vel.y * vel.y);
    return speed < 0.5f;
}