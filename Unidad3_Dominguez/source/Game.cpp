#include "Game.h"
#include <string> 

Vector2 Game::ToScreen(b2Vec2 worldPos) {
    Vector2 screenPos = { 0.0f, 0.0f };
    screenPos.x = worldPos.x * 45.0f;
    screenPos.y = 800.0f - (worldPos.y * 45.0f);
    return screenPos;
}

Game::Game() : world(nullptr), ground(nullptr), startPlatform(nullptr),
cajaRoja(nullptr), cajaAzul(nullptr), bridge(nullptr),
elevator(nullptr), azulActiva(true) {

    InitWindow(1400, 800, "Laboratorio Mecanico - Mavix");
    SetTargetFPS(60);

    world = new b2World(b2Vec2(0.0f, -18.0f));

    b2BodyDef groundDef;
    groundDef.position.Set(15.0f, 0.5f);
    ground = world->CreateBody(&groundDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(30.0f, 0.5f);
    ground->CreateFixture(&groundBox, 0.0f);

    b2BodyDef platDef;
    platDef.position.Set(2.75f, 9.0f);
    startPlatform = world->CreateBody(&platDef);
    b2PolygonShape platShape;
    platShape.SetAsBox(2.75f, 0.4f);
    startPlatform->CreateFixture(&platShape, 0.0f);

    bridge = new SuspensionBridge(world, 7.5f, 19.5f, 9.0f, 1.5f, 0.2f);
    elevator = new Elevator(world, ground, 22.2f, 9.0f);

    cajaRoja = new BoxPlayer(world, 1.5f, 9.0f + 0.65f, RED);
    cajaAzul = new BoxPlayer(world, 4.0f, 9.0f + 0.65f, DARKBLUE);
}

Game::~Game() {
    delete cajaRoja;
    delete cajaAzul;
    delete bridge;
    delete elevator;
    delete world;
    CloseWindow();
}

void Game::HandleInput() {
    BoxPlayer* current = azulActiva ? cajaAzul : cajaRoja;

    if (IsKeyDown(KEY_RIGHT)) current->ApplyHorizontalForce(38.0f);
    if (IsKeyDown(KEY_LEFT)) current->ApplyHorizontalForce(-38.0f);

    current->LimitVelocity(9.0f);

    if (IsKeyPressed(KEY_UP) && current->IsGrounded()) {
        current->Jump(13.0f);
    }

    if (IsKeyPressed(KEY_SPACE)) {
        azulActiva = !azulActiva;
    }

    if (IsKeyPressed(KEY_R)) {
        cajaRoja->Reset();
        cajaAzul->Reset();
        elevator->Reset(22.2f, 9.0f);
    }
}

void Game::ApplyScreenLimits(BoxPlayer* player) {
    const float leftLimit = 0.5f;
    const float rightLimit = 1400.0f / 45.0f - 0.5f;
    const float topLimit = 800.0f / 45.0f - 0.6f;

    b2Vec2 pos = player->GetPosition();
    if (pos.x < leftLimit) {
        player->GetBody()->SetTransform(b2Vec2(leftLimit, pos.y), 0);
        player->GetBody()->SetLinearVelocity(b2Vec2(0, player->GetBody()->GetLinearVelocity().y));
    }
    if (pos.x > rightLimit) {
        player->GetBody()->SetTransform(b2Vec2(rightLimit, pos.y), 0);
        player->GetBody()->SetLinearVelocity(b2Vec2(0, player->GetBody()->GetLinearVelocity().y));
    }
    if (pos.y > topLimit) {
        player->GetBody()->SetTransform(b2Vec2(pos.x, topLimit), 0);
        player->GetBody()->SetLinearVelocity(b2Vec2(player->GetBody()->GetLinearVelocity().x, 0));
    }
}

void Game::Run() {
    float timeStep = 1.0f / 60.0f;
    int velIter = 8, posIter = 3;

    while (!WindowShouldClose()) {
        HandleInput();

        world->Step(timeStep, velIter, posIter);

        cajaRoja->HandleVacuumFall();
        cajaAzul->HandleVacuumFall();

        elevator->UpdateSensors(cajaRoja->GetPosition(), cajaAzul->GetPosition());
        elevator->UpdatePhysics();

        ApplyScreenLimits(cajaRoja);
        ApplyScreenLimits(cajaAzul);

        BeginDrawing();
        ClearBackground(SKYBLUE);

        DrawRectangle(0, 800 - static_cast<int>(1.0f * 45.0f), 1400, static_cast<int>(1.0f * 45.0f), DARKGRAY);

        Vector2 startPos = ToScreen(b2Vec2(2.75f, 9.0f));
        DrawRectangle(static_cast<int>(startPos.x - 2.75f * 45.0f), static_cast<int>(startPos.y - 0.4f * 45.0f),
            static_cast<int>(5.5f * 45.0f), static_cast<int>(0.8f * 45.0f), LIGHTGRAY);

        bridge->Draw(45.0f, Game::ToScreen);
        elevator->Draw(45.0f, Game::ToScreen);
        cajaRoja->Draw(45.0f, Game::ToScreen);
        cajaAzul->Draw(45.0f, Game::ToScreen);

        DrawText("Controles: Flechas (Mover) | Arriba (Saltar) | Espacio (Cambiar Caja) | R (Reset)", 10, 10, 20, BLACK);
        std::string activeMsg = "Caja Activa: " + std::string(azulActiva ? "AZUL" : "ROJA");
        DrawText(activeMsg.c_str(), 10, 40, 20, azulActiva ? DARKBLUE : RED);

        EndDrawing();
    }
}