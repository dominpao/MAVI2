#include "Goal.h"
#include <raylib.h>
#include <cmath>
#include <cstdio>

Goal::Goal() : body(nullptr) {}

Goal::~Goal() {}

void Goal::crear(b2World* world, std::map<b2Body*, int>& bodyIDs) {
    float ppm = 30.0f;
    float arcoX = (float)(800 - 20) / ppm;
    float arcoY = (float)(600 / 2) / ppm;

    b2BodyDef arcoDef;
    arcoDef.type = b2_staticBody;
    arcoDef.position.Set(arcoX, arcoY);
    body = world->CreateBody(&arcoDef);

    bodyIDs[body] = 3;

    b2PolygonShape arcoShape;
    arcoShape.SetAsBox(0.3f, (float)600 / ppm / 2);

    b2FixtureDef arcoFix;
    arcoFix.shape = &arcoShape;
    arcoFix.isSensor = true;
    body->CreateFixture(&arcoFix);
}

void Goal::dibujar(float ppm, int screenHeight, bool pelotaActiva, bool running) {
    int screenWidth = 800;
    float goalX = (float)(screenWidth - 70);
    float goalY = 0.0f;
    float goalWidth = 50;
    float goalHeight = (float)screenHeight;

    DrawRectangle((int)goalX, (int)goalY, (int)goalWidth, (int)goalHeight, ColorAlpha(WHITE, 0.05f));

    for (int i = 0; i < 10; i++) {
        DrawLine((int)goalX + i * 5, (int)goalY, (int)goalX + i * 5, (int)goalY + (int)goalHeight, ColorAlpha(WHITE, 0.08f));
    }
    for (int i = 0; i < 60; i++) {
        DrawLine((int)goalX, (int)goalY + i * 10, (int)goalX + (int)goalWidth, (int)goalY + i * 10, ColorAlpha(WHITE, 0.08f));
    }

    DrawRectangle((int)goalX - 6, (int)goalY, 6, (int)goalHeight, WHITE);
    DrawRectangle((int)goalX + (int)goalWidth, (int)goalY, 6, (int)goalHeight, WHITE);
    DrawRectangle((int)goalX - 6, (int)goalY, (int)goalWidth + 12, 6, WHITE);
    DrawRectangle((int)goalX - 6, (int)goalY + (int)goalHeight - 6, (int)goalWidth + 12, 6, WHITE);

    DrawRectangle((int)goalX - 4, (int)goalY + 6, 4, (int)goalHeight - 12, ColorAlpha(WHITE, 0.3f));
    DrawRectangle((int)goalX + (int)goalWidth, (int)goalY + 6, 4, (int)goalHeight - 12, ColorAlpha(WHITE, 0.3f));

    DrawRectangle((int)goalX - 5, (int)goalY + 10, (int)goalWidth + 10, 28, ColorAlpha(BLACK, 0.7f));
    DrawText("GOAL", (int)goalX + 5, (int)goalY + 15, 20, RED);

    if (pelotaActiva && running) {
        float alpha = 0.05f + 0.08f * sin(GetTime() * 4);
        DrawRectangle((int)goalX, (int)goalY, (int)goalWidth, (int)goalHeight, ColorAlpha(RED, alpha));
    }
}