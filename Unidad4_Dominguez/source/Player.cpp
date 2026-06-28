#include "Player.h"
#include <cmath>

Player::Player()
    : body(nullptr)
    , spawnPosition(b2Vec2_zero)
    , lastCheckpointPos(b2Vec2_zero)
    , checkpointUnlocked(false)
    , grounded(false)
    , onPlatform(false)
{
}

Player::~Player() {
}

// Init: Crea el cuerpo dinamico del jugador en Box2D con su colisionador cuadrado y asigna UserData tipo PLAYER
void Player::Init(b2World* world, b2Vec2 spawnPos) {
    spawnPosition = spawnPos;
    lastCheckpointPos = spawnPos;

    b2BodyDef pDef;
    pDef.type = b2_dynamicBody;
    pDef.position.Set(spawnPosition.x, spawnPosition.y);
    pDef.fixedRotation = true;
    body = world->CreateBody(&pDef);

    b2PolygonShape poly;
    float capyHalfSize = (CAPY_SIZE * 0.7f / 2.0f) / SCALE;
    poly.SetAsBox(capyHalfSize, capyHalfSize);

    b2FixtureDef fd;
    fd.shape = &poly;
    fd.density = 1.0f;
    fd.friction = 0.9f;
    fd.restitution = 0.0f;
    body->CreateFixture(&fd);

    UserData* data = new UserData();
    data->type = BodyType::PLAYER;
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
}

// MoveRight: Fija velocidad horizontal positiva (derecha)
void Player::MoveRight() {
    b2Vec2 vel = body->GetLinearVelocity();
    vel.x = MOVE_SPEED;
    body->SetLinearVelocity(vel);
}

// MoveLeft: Fija velocidad horizontal negativa (izquierda)
void Player::MoveLeft() {
    b2Vec2 vel = body->GetLinearVelocity();
    vel.x = -MOVE_SPEED;
    body->SetLinearVelocity(vel);
}

// Stop: Aplica friccion horizontal (85% cada frame) y corta si es muy baja
void Player::Stop() {
    b2Vec2 vel = body->GetLinearVelocity();
    vel.x *= 0.85f;
    if (fabs(vel.x) < 0.1f) vel.x = 0;
    body->SetLinearVelocity(vel);
}

// Jump: Fija velocidad vertical negativa (salta hacia arriba)
void Player::Jump() {
    b2Vec2 vel = body->GetLinearVelocity();
    vel.y = JUMP_FORCE;
    body->SetLinearVelocity(vel);
}

// CutJump: Reduce la velocidad vertical si el jugador solto la tecla
//          (permite saltos de altura variable)
void Player::CutJump() {
    b2Vec2 vel = body->GetLinearVelocity();
    if (vel.y < -5.0f) {
        vel.y *= 0.7f;
        body->SetLinearVelocity(vel);
    }
}

// Respawn: Teletransporta al jugador al ultimo checkpoint o al spawn inicial si no tiene ninguno desbloqueado
void Player::Respawn() {
    b2Vec2 respawnPos = checkpointUnlocked ? lastCheckpointPos : spawnPosition;
    body->SetTransform(respawnPos, 0);
    body->SetLinearVelocity(b2Vec2(0, 0));
}

// ResetToSpawn: Vuelve al spawn, borra checkpoint y flags (se usa con la tecla R)
void Player::ResetToSpawn() {
    body->SetTransform(spawnPosition, 0);
    body->SetLinearVelocity(b2Vec2(0, 0));
    checkpointUnlocked = false;
    lastCheckpointPos = spawnPosition;
    grounded = false;
    onPlatform = false;
}

void Player::UpdateGroundStatus(bool isGrounded) {
    grounded = isGrounded;
}

void Player::UpdateOnPlatform(bool isOnPlatform) {
    onPlatform = isOnPlatform;
}

b2Body* Player::GetBody() const {
    return body;
}

b2Vec2 Player::GetPosition() const {
    return body->GetPosition();
}

b2Vec2 Player::GetVelocity() const {
    return body->GetLinearVelocity();
}

bool Player::IsCheckpointUnlocked() const {
    return checkpointUnlocked;
}

b2Vec2 Player::GetLastCheckpointPos() const {
    return lastCheckpointPos;
}

bool Player::IsOnPlatform() const {
    return onPlatform;
}

bool Player::IsGrounded() const {
    return grounded;
}

void Player::SetCheckpointUnlocked(bool unlocked) {
    checkpointUnlocked = unlocked;
}

void Player::SetLastCheckpointPos(b2Vec2 pos) {
    lastCheckpointPos = pos;
}

void Player::SetSpawnPosition(b2Vec2 pos) {
    spawnPosition = pos;
}
