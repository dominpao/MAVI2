#ifndef NDEBUG#ifndef NDEBUG
#include <vld.h>
#endif

#include "raylib.h"
#include <box2d.h>
#include <vector>
#include <cmath>

struct PhysicsBox
{
    b2Body* body;
    float width;
    float height;
    Color color;
};

struct PhysicsCircle
{
    b2Body* body;
    float radius;
    Color color;
};

int main(void)
{
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "MAVI II - Creacion dinamica de cajas");
    SetTargetFPS(60);

    Color fondo = { 110, 100, 215, 255 };
    Color textoPrincipal = RAYWHITE;
    Color textoSecundario = DARKPURPLE;
    Color sueloColor = Fade(DARKGREEN, 0.7f);

    // Mundo fisico
    b2Vec2 gravity(0.0f, 9.8f);
    b2World world(gravity);

    // Suelo estatico
    b2BodyDef groundDef{};
    groundDef.type = b2_staticBody;
    groundDef.position.Set(screenWidth / 2.0f, screenHeight - 40.0f);
    b2Body* groundBody = world.CreateBody(&groundDef);
    b2PolygonShape groundShape{};
    groundShape.SetAsBox(screenWidth / 2.0f, 20.0f);
    groundBody->CreateFixture(&groundShape, 0.0f);

    std::vector<PhysicsBox> boxes;
    std::vector<PhysicsCircle> circles;

    // Cajas iniciales 
    for (int i = 0; i < 4; i++)
    {
        b2BodyDef boxDef{};
        boxDef.type = b2_dynamicBody;
        boxDef.position.Set(300.0f + i * 80.0f, 80.0f + i * 20.0f);
        b2Body* boxBody = world.CreateBody(&boxDef);
        b2PolygonShape boxShape{};
        boxShape.SetAsBox(25.0f, 25.0f);
        b2FixtureDef boxFixture{};
        boxFixture.shape = &boxShape;
        boxFixture.density = 1.0f;
        boxFixture.friction = 0.4f;
        boxFixture.restitution = 0.2f;
        boxBody->CreateFixture(&boxFixture);
        boxes.push_back({ boxBody, 50.0f, 50.0f, Fade(SKYBLUE, 0.95f) });
    }

    // Circulos iniciales
    for (int i = 0; i < 3; i++)
    {
        b2BodyDef circleDef{};
        circleDef.type = b2_dynamicBody;
        circleDef.position.Set(650.0f + i * 60.0f, 60.0f + i * 30.0f);
        b2Body* circleBody = world.CreateBody(&circleDef);
        b2CircleShape circleShape{};
        circleShape.m_radius = 20.0f;
        b2FixtureDef circleFixture{};
        circleFixture.shape = &circleShape;
        circleFixture.density = 1.0f;
        circleFixture.friction = 0.3f;
        circleFixture.restitution = 0.6f;
        circleBody->CreateFixture(&circleFixture);
        circles.push_back({ circleBody, 20.0f, Fade(ORANGE, 0.95f) });
    }

    // Variables para rotacion previa y spawn
    float spawnAngleDeg = 0.0f;
    const float ANGLE_STEP = 10.0f;
    const float spawnX = screenWidth / 2.0f;
    const float spawnY = 80.0f;
    const float boxSize = 50.0f;   // misma que las iniciales
    Color newBoxColor = Fade(MAGENTA, 0.95f);

    while (!WindowShouldClose())
    {
        // Rotacion con flechas (10° por pulsacion)
        if (IsKeyPressed(KEY_LEFT))
            spawnAngleDeg -= ANGLE_STEP;
        if (IsKeyPressed(KEY_RIGHT))
            spawnAngleDeg += ANGLE_STEP;

        // Crear nueva caja 
        if (IsKeyPressed(KEY_SPACE))
        {
            float spawnAngleRad = spawnAngleDeg * DEG2RAD;
            b2BodyDef boxDef{};
            boxDef.type = b2_dynamicBody;
            boxDef.position.Set(spawnX, spawnY);
            boxDef.angle = spawnAngleRad;
            b2Body* newBoxBody = world.CreateBody(&boxDef);
            b2PolygonShape boxShape{};
            boxShape.SetAsBox(boxSize / 2.0f, boxSize / 2.0f);
            b2FixtureDef boxFixture{};
            boxFixture.shape = &boxShape;
            boxFixture.density = 1.0f;
            boxFixture.friction = 0.4f;
            boxFixture.restitution = 0.2f;
            newBoxBody->CreateFixture(&boxFixture);
            boxes.push_back({ newBoxBody, boxSize, boxSize, newBoxColor });
        }

        world.Step(1.0f / 60.0f, 8, 3);

        BeginDrawing();
        ClearBackground(fondo);
        DrawRectangle(0, screenHeight - 60, screenWidth, 40, sueloColor);

        // Dibujar todas las cajas con el mismo estilo
        for (const auto& box : boxes)
        {
            b2Vec2 pos = box.body->GetPosition();
            float angleRad = box.body->GetAngle();
            float angleDeg = angleRad * RAD2DEG;

            Rectangle rect = { pos.x - box.width / 2, pos.y - box.height / 2, box.width, box.height };
            Vector2 origin = { box.width / 2, box.height / 2 };
            DrawRectanglePro(rect, origin, angleDeg, box.color);
            DrawRectangleLinesEx(rect, 2, DARKBLUE);  

            // Linea roja desde el centro hacia la derecha local
            float tipX = pos.x + (box.width / 2) * cosf(angleRad);
            float tipY = pos.y + (box.width / 2) * sinf(angleRad);
            DrawLineEx({ pos.x, pos.y }, { tipX, tipY }, 2.0f, RED);
        }

        // Dibujar circulos
        for (const auto& circle : circles)
        {
            b2Vec2 pos = circle.body->GetPosition();
            DrawCircleV({ pos.x, pos.y }, circle.radius, circle.color);
            DrawCircleLines((int)pos.x, (int)pos.y, circle.radius, BROWN);
        }

        // UI 
        DrawRectangle(90, 30, 820, 120, Fade(BLACK, 0.18f));
        float displayAngle = fmod(spawnAngleDeg, 360.0f);
        if (displayAngle < 0) displayAngle += 360.0f;
        const char* angleText = TextFormat("Rotacion nueva caja: %.0f°  (Flecha Izquierda/Derecha)", displayAngle, (int)ANGLE_STEP);
        DrawText(angleText, 120, 50, 20, RAYWHITE);
        DrawText("Presiona ESPACIO para crear una caja con esa rotacion", 120, 90, 20, DARKPURPLE);
        DrawText("Creacion dinamica de cajas con rotacion inicial", 300, 540, 20, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}