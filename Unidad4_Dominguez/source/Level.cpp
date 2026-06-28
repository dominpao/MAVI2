#include "Level.h"
#include <cmath>
#include <iostream>

// Constructor: Inicializa todos los punteros a nullptr y flags
Level::Level()
    : world(nullptr)
    , player(nullptr)
    , contactListener(nullptr)
    , platformMoving(false)
    , platformFinished(false)
    , moveDirection(1.0f)
    , finalPlatformInitialPos(b2Vec2_zero)
{
}

// Destructor: Libera UserData, plataformas, player, listener y mundo en ese orden (inverso al de creacion)
Level::~Level() {
    for (auto* p : platforms) {
        if (p->GetBody()) {
            auto* data = reinterpret_cast<UserData*>(p->GetBody()->GetUserData().pointer);
            delete data;
        }
        delete p;
    }
    platforms.clear();

    if (player && player->GetBody()) {
        auto* data = reinterpret_cast<UserData*>(player->GetBody()->GetUserData().pointer);
        delete data;
    }
    delete player;
    delete contactListener;
    delete world;
}

// Init: Crea el mundo Box2D, el contact listener, el jugador y las plataformas del nivel
void Level::Init() {
    world = new b2World(b2Vec2(0.0f, 18.0f));
    contactListener = new ContactListener();
    world->SetContactListener(contactListener);

    player = new Player();
    b2Vec2 spawnPosition(200 / SCALE, (700 - PLAT_H / 2 - CAPY_SIZE / 2) / SCALE);
    player->Init(world, spawnPosition);

    CreatePlatforms();
}

// CreatePlatforms: Define las 7 plataformas en escalera.
//                  La de la posicion 950,400 es CHECKPOINT,
//                  la de 200,100 es la FINAL (se mueve al ganar)
void Level::CreatePlatforms() {
    struct PlatDef {
        float x, y;
        BodyType type;
    };

    PlatDef defs[] = {
        {200, 700, BodyType::PLATFORM},
        {450, 600, BodyType::PLATFORM},
        {700, 500, BodyType::PLATFORM},
        {950, 400, BodyType::SENSOR_CHECKPOINT},
        {700, 300, BodyType::PLATFORM},
        {450, 200, BodyType::PLATFORM},
        {200, 100, BodyType::FINAL_PLATFORM}
    };

    for (auto& d : defs) {
        auto* platform = new Platform();
        platform->Init(world, d.x, d.y, d.type);
        platforms.push_back(platform);

        if (d.type == BodyType::FINAL_PLATFORM) {
            finalPlatformInitialPos = b2Vec2(d.x / SCALE, d.y / SCALE);
        }
    }
}

// Step: Avanza la fisica un frame. Luego detecta si el jugador toca el suelo.
// Si la plataforma final esta activa la mueve
void Level::Step(float dt) {
    world->Step(dt, 8, 3);
    DetectGround();

    if (platformMoving && !platformFinished) {
        UpdateFinalPlatform();
    }
}

// DetectGround: Si el borde inferior del jugador esta cerca del borde superior de una plataforma,
//               se considera que esta en suelo firme
void Level::DetectGround() {
    b2Vec2 pos = player->GetPosition();
    b2Vec2 vel = player->GetVelocity();
    bool isGrounded = false;

    if (fabs(vel.y) < 0.5f) {
        for (auto* p : platforms) {
            float platTop = (p->GetY() - PLAT_H / 2) / SCALE;
            float platLeft = (p->GetX() - PLAT_W / 2 * 0.8f) / SCALE;
            float platRight = (p->GetX() + PLAT_W / 2 * 0.8f) / SCALE;

            float playerBottom = pos.y + (CAPY_SIZE / 2 * 0.7f) / SCALE;
            float playerLeft = pos.x - (CAPY_SIZE / 2 * 0.7f) / SCALE;
            float playerRight = pos.x + (CAPY_SIZE / 2 * 0.7f) / SCALE;

            if (playerBottom >= platTop - 0.05f &&
                playerBottom <= platTop + 0.3f &&
                playerRight > platLeft + 0.1f &&
                playerLeft < platRight - 0.1f) {
                isGrounded = true;
                break;
            }
        }
    }

    player->UpdateGroundStatus(isGrounded);
}

// UpdateFinalPlatform: Mueve la plataforma final hacia la derecha.
//                      Si el jugador esta sobre ella lo arrastra.
//                      Al llegar al limite marca platformFinished como true
                     
void Level::UpdateFinalPlatform() {
    auto* finalPlat = GetFinalPlatform();
    if (!finalPlat) return;

    b2Body* platBody = finalPlat->GetBody();
    b2Vec2 platformPos = platBody->GetPosition();
    b2Vec2 capyPos = player->GetPosition();

    float rightLimit = (SCREEN_WIDTH - PLAT_W - 30) / SCALE;
    platformPos.x += PLATFORM_SPEED * moveDirection;

    if (platformPos.x >= rightLimit) {
        platformPos.x = rightLimit;
        platformFinished = true;
        platformMoving = false;
        std::cout << "Plataforma llego al final!" << std::endl;
    }

    platBody->SetTransform(platformPos, 0);
    finalPlat->UpdatePositionFromBody();

    float distY = fabs(capyPos.y - (platformPos.y - PLAT_H / 2 / SCALE - CAPY_SIZE / 2 * 0.7f / SCALE));
    bool onPlat = contactListener->playerOnFinalPlatform || distY < 0.5f;

    if (onPlat) {
        player->UpdateOnPlatform(true);
        player->UpdateGroundStatus(true);

        float targetY = platformPos.y - PLAT_H / 2 / SCALE - CAPY_SIZE / 2 * 0.7f / SCALE;
        player->GetBody()->SetTransform(b2Vec2(platformPos.x, targetY), 0);
        player->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
    } else {
        player->UpdateOnPlatform(false);
    }
}

// RespawnIfFallen: Si el jugador cae por debajo de y=30
//                  lo regresa al ultimo checkpoint o al spawn si no tiene ninguno

void Level::RespawnIfFallen() {
    b2Vec2 pos = player->GetPosition();
    if (pos.y > 30.0f && !player->IsOnPlatform()) {
        player->Respawn();
        std::cout << "Caida! Respawn en " << (player->IsCheckpointUnlocked() ? "checkpoint" : "inicio") << std::endl;
    }
}

bool Level::HasPlayerFallen() const {
    b2Vec2 pos = player->GetPosition();
    return pos.y > 30.0f && !player->IsOnPlatform();
}

void Level::Reset() {
    if (auto* fp = GetFinalPlatform()) {
        fp->GetBody()->SetTransform(finalPlatformInitialPos, 0);
        fp->SetX(finalPlatformInitialPos.x * SCALE);
    }
    platformMoving = false;
    platformFinished = false;
    moveDirection = 1.0f;

    contactListener->checkpointReached = false;
    contactListener->goalReached = false;
    contactListener->checkpointActive = false;
    contactListener->playerOnFinalPlatform = false;

    player->ResetToSpawn();
}

void Level::StartFinalPlatform() {
    platformMoving = true;
    platformFinished = false;
    moveDirection = 1.0f;
}

bool Level::IsPlatformMoving() const {
    return platformMoving;
}

bool Level::IsPlatformFinished() const {
    return platformFinished;
}

Platform* Level::GetFinalPlatform() const {
    for (auto* p : platforms) {
        if (p->IsFinal()) return p;
    }
    return nullptr;
}

Player* Level::GetPlayer() const {
    return player;
}

std::vector<Platform*>& Level::GetPlatforms() {
    return platforms;
}

ContactListener* Level::GetContactListener() const {
    return contactListener;
}

b2World* Level::GetWorld() const {
    return world;
}
