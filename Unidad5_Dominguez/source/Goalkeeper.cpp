#include "Goalkeeper.h"
#include <cstdio>

Goalkeeper::Goalkeeper() : body(nullptr) {
    // Carga la textura del arquero desde archivo
    texture = LoadTexture("arquero.png");
    if (texture.id == 0) {
        printf("Warning: arquero.png not found\n");
    }
}

Goalkeeper::~Goalkeeper() {
    if (texture.id != 0) {
        UnloadTexture(texture);
    }
}

void Goalkeeper::crear(b2World* world, std::map<b2Body*, int>& bodyIDs) {
    // Crea el cuerpo fisico del arquero en Box2D
    float ppm = 30.0f;

    float arqueroX = (float)(800 - 35) / ppm;
    float arqueroY = (float)(600 / 2) / ppm;

    b2BodyDef arqueroDef;
    arqueroDef.type = b2_dynamicBody;
    arqueroDef.position.Set(arqueroX, arqueroY);
    arqueroDef.fixedRotation = true;
    body = world->CreateBody(&arqueroDef);

    // ID 2 = arquero para el contact listener
    bodyIDs[body] = 2;

    b2PolygonShape arqueroShape;
    arqueroShape.SetAsBox(0.4f, 0.9f);

    b2FixtureDef arqueroFix;
    arqueroFix.shape = &arqueroShape;
    arqueroFix.density = 1.0f;
    arqueroFix.friction = 0.5f;
    arqueroFix.restitution = 0.1f;
    body->CreateFixture(&arqueroFix);

    posicionInicial = b2Vec2(arqueroX, arqueroY);
}

void Goalkeeper::update(float ppm, int screenHeight) {
    // Mueve al arquero con las flechas arriba/abajo o W/S
    if (body == nullptr) return;

    float moveSpeed = 7.0f;
    b2Vec2 vel = body->GetLinearVelocity();

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        vel.y = moveSpeed;
    }
    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        vel.y = -moveSpeed;
    }
    else {
        vel.y *= 0.9f;
    }
    body->SetLinearVelocity(vel);

    // Limita la posicion del arquero dentro de la pantalla
    b2Vec2 pos = body->GetPosition();
    float minY = 50.0f / ppm;
    float maxY = (float)(screenHeight - 50) / ppm;

    if (pos.y < minY) {
        pos.y = minY;
        body->SetTransform(pos, 0);
    }
    if (pos.y > maxY) {
        pos.y = maxY;
        body->SetTransform(pos, 0);
    }
}

void Goalkeeper::dibujar(float ppm, int screenHeight) {
    // Dibuja al arquero usando textura o forma geometrica por defecto
    if (body == nullptr) return;

    b2Vec2 pos = body->GetPosition();
    float screenX = pos.x * ppm;
    float screenY = screenHeight - pos.y * ppm;

    float goalX = (float)(800 - 70);
    float goalWidth = 50;

    // Limita la posicion visual del arquero dentro del area del arco
    if (screenX > goalX + goalWidth - 5) screenX = goalX + goalWidth - 5;
    if (screenX < goalX + 5) screenX = goalX + 5;
    if (screenY < 25) screenY = 25;
    if (screenY > screenHeight - 25) screenY = screenHeight - 25;

    if (texture.id != 0) {
        Rectangle destRect = { screenX - 25, screenY - 35, 50, 70 };
        DrawTexturePro(texture,
            { 0, 0, (float)texture.width, (float)texture.height },
            destRect, { 0, 0 }, 0, WHITE);
    }
    else {
        // Cuerpo simple si no hay textura disponible
        DrawRectanglePro({ screenX, screenY, 30, 45 }, { 15, 22 }, 0, BLUE);
        DrawCircle((int)screenX - 30, (int)screenY - 5, 14, BLUE);
        DrawCircle((int)screenX + 30, (int)screenY - 5, 14, BLUE);
        DrawCircle((int)screenX - 30, (int)screenY - 5, 9, YELLOW);
        DrawCircle((int)screenX + 30, (int)screenY - 5, 9, YELLOW);
        DrawCircle((int)screenX, (int)screenY - 32, 15, BLUE);
        DrawCircle((int)screenX, (int)screenY - 32, 11, ColorAlpha(WHITE, 0.5f));
        DrawCircle((int)screenX - 5, (int)screenY - 35, 3, BLACK);
        DrawCircle((int)screenX + 5, (int)screenY - 35, 3, BLACK);
        DrawLine((int)screenX - 4, (int)screenY - 27, (int)screenX + 4, (int)screenY - 27, BLACK);
    }

    // Etiqueta GK sobre el arquero
    DrawRectangle((int)screenX - 28, (int)screenY - 60, 56, 20, ColorAlpha(BLACK, 0.7f));
    DrawText("GK", (int)screenX - 10, (int)screenY - 56, 16, WHITE);
}

void Goalkeeper::resetPosition() {
    // Vuelve al arquero a su posicion inicial en el centro del arco
    if (body == nullptr) return;
    body->SetTransform(posicionInicial, 0);
    body->SetLinearVelocity(b2Vec2(0, 0));
}

void Goalkeeper::freeze() {
    // Detiene por completo el movimiento del arquero
    if (body == nullptr) return;
    body->SetLinearVelocity(b2Vec2(0, 0));
    body->SetTransform(body->GetPosition(), 0);
}