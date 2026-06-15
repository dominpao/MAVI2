#include "Player.h"
#include "Renderer.h"

Player::Player(b2World* world, float x, float y, Color color)
    : isActivePlayer(false), playerColor(color), onGround(false) {
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(x, y);
    def.fixedRotation = true;
    def.bullet = true;
    def.linearDamping = 0.3f;
    body = world->CreateBody(&def);

    // Collider mas pequeno que el visual para mejor jugabilidad
    b2PolygonShape box;
    box.SetAsBox(0.35f, 0.45f);

    b2FixtureDef fix;
    fix.shape = &box;
    fix.density = 1.2f;
    fix.friction = 0.6f;
    fix.restitution = 0.0f;
    body->CreateFixture(&fix);
}

Player::~Player() {}

void Player::moveLeft(float force) {
    if (isActivePlayer) {
        body->ApplyForceToCenter(b2Vec2(-force, 0), true);
        b2Vec2 vel = body->GetLinearVelocity();
        if (vel.x < -6.0f) body->SetLinearVelocity(b2Vec2(-6.0f, vel.y));
    }
}

void Player::moveRight(float force) {
    if (isActivePlayer) {
        body->ApplyForceToCenter(b2Vec2(force, 0), true);
        b2Vec2 vel = body->GetLinearVelocity();
        if (vel.x > 6.0f) body->SetLinearVelocity(b2Vec2(6.0f, vel.y));
    }
}

void Player::jump(float impulse) {
    if (!isActivePlayer) return;

    updateGroundStatus();

    if (onGround) {
        // Limpiar velocidad vertical antes de saltar para salto consistente
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, 0));
        body->ApplyLinearImpulseToCenter(b2Vec2(0, impulse), true);
        onGround = false;
    }
}

void Player::reset(float x, float y) {
    body->SetTransform(b2Vec2(x, y), 0);
    body->SetLinearVelocity(b2Vec2(0, 0));
    body->SetAwake(true);
    onGround = false;
}

// Detecta si el personaje esta tocando el suelo
void Player::updateGroundStatus() {
    bool contacto = false;
    for (b2ContactEdge* ce = body->GetContactList(); ce != nullptr; ce = ce->next) {
        if (ce->contact->IsTouching()) {
            contacto = true;
            break;
        }
    }
    onGround = contacto;
}

bool Player::getIsOnGround() { return onGround; }
void Player::setActive(bool active) { isActivePlayer = active; }
bool Player::isActive() { return isActivePlayer; }
b2Vec2 Player::getPosition() { return body->GetPosition(); }
b2Body* Player::getBody() { return body; }

// Verifica si el personaje esta sobre la plataforma meta (dorada)
bool Player::isInMeta(float metaX, float minY, float maxY) {
    b2Vec2 pos = body->GetPosition();
    return (pos.x > metaX && pos.y > minY && pos.y < maxY);
}

void Player::draw(Renderer* renderer) {
    renderer->drawPlayer(body->GetPosition(), playerColor, false);
}