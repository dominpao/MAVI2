#include "Game.h"
#include "Constants.h"

Game::Game() : physicsWorld(), projectile(physicsWorld.getWorld()) {
    InitWindow(static_cast<int>(SCREEN_WIDTH), static_cast<int>(SCREEN_HEIGHT), "MAVI II - Catapulta POO");
    SetTargetFPS(60);
    createObjects();

    Vector2 spoon = catapult.getSpoonPosition();
    projectile.updatePositionToSpoon(spoon);
}

Game::~Game() {
    CloseWindow();
}

void Game::createObjects() {
    for (int i = 0; i < 4; ++i) {
        float x = 300.0f + i * 80.0f;
        float y = 80.0f + i * 20.0f;
        boxes.emplace_back(physicsWorld.getWorld(), x, y, 50.0f, 50.0f, Fade(SKYBLUE, 0.95f));
    }
    for (int i = 0; i < 3; ++i) {
        float x = 650.0f + i * 60.0f;
        float y = 60.0f + i * 30.0f;
        circles.emplace_back(physicsWorld.getWorld(), x, y, 20.0f, Fade(ORANGE, 0.95f));
    }
}

void Game::resetAll() {
    projectile.reset();
    catapult.reset();
    for (auto& box : boxes) box.reset();
    for (auto& circle : circles) circle.reset();
}

void Game::run() {
    while (!WindowShouldClose()) {
        processInput();
        update();
        draw();
    }
}

void Game::processInput() {
    if (IsKeyPressed(KEY_R)) {
        resetAll();
    }
}

void Game::update() {
    bool resting = projectile.isResting();
    bool launchTrigger = false;
    b2Vec2 impulse;
    Vector2 spoonPos;

    catapult.update(GetFrameTime(), resting, launchTrigger, impulse, spoonPos);
    projectile.updatePositionToSpoon(spoonPos);

    if (launchTrigger) {
        projectile.launch(impulse);
    }

    physicsWorld.step();
}

void Game::draw() const {
    BeginDrawing();
    ClearBackground({ 110, 100, 215, 255 });

    DrawRectangle(0, static_cast<int>(GROUND_Y), static_cast<int>(SCREEN_WIDTH), static_cast<int>(GROUND_HEIGHT), Fade(DARKGREEN, 0.7f));

    for (const auto& box : boxes) box.draw();
    for (const auto& circle : circles) circle.draw();

    catapult.draw();
    projectile.draw();

    if (catapult.isCharging()) {
        float percent = catapult.getChargePercent();
        int barWidth = 200;
        int barHeight = 20;
        int barX = static_cast<int>(SCREEN_WIDTH) / 2 - barWidth / 2;
        int barY = static_cast<int>(SCREEN_HEIGHT) - 100;
        DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
        DrawRectangle(barX, barY, static_cast<int>(barWidth * percent), barHeight, RED);
        DrawText("Cargando... Manten ESPACIO", barX + 10, barY - 25, 20, YELLOW);
    }

    DrawRectangle(90, 70, 820, 120, Fade(BLACK, 0.18f));
    DrawText("Manten ESPACIO: tensa el brazo. Suelta para lanzar.", 120, 100, 20, RAYWHITE);
    DrawText("Presiona R para reiniciar.", 120, 135, 20, RAYWHITE);

    EndDrawing();
}