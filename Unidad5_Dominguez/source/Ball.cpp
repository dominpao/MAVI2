#include "Ball.h"
#include <raylib.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>

Ball::Ball() : body(nullptr), activa(false) {}

Ball::~Ball() {}

void Ball::crear(b2World* world, std::map<b2Body*, int>& bodyIDs) {
    // Elimina la pelota anterior si existe
    if (body != nullptr) {
        bodyIDs.erase(body);
        world->DestroyBody(body);
        body = nullptr;
    }

    // Posicion aleatoria dentro del campo
    float posX = (float)(30 + rand() % 80) / 30.0f;
    float posY = (float)(50 + rand() % 500) / 30.0f;

    b2BodyDef pelotaDef;
    pelotaDef.type = b2_dynamicBody;
    pelotaDef.position.Set(posX, posY);
    body = world->CreateBody(&pelotaDef);

    // ID 1 = pelota para el contact listener
    bodyIDs[body] = 1;

    b2CircleShape pelotaShape;
    pelotaShape.m_radius = 0.25f;

    b2FixtureDef pelotaFix;
    pelotaFix.shape = &pelotaShape;
    pelotaFix.density = 0.5f;
    pelotaFix.friction = 0.3f;
    pelotaFix.restitution = 0.6f;
    body->CreateFixture(&pelotaFix);

    // Velocidad aleatoria para el tiro
    float velX = 10.0f + (float)(rand() % 8);
    float velY = (float)(-8 + rand() % 17);

    if (velY > -1 && velY < 1) {
        velY = (float)(-3 + rand() % 7);
    }

    body->SetLinearVelocity(b2Vec2(velX, velY));
    body->SetGravityScale(0.0f);

    activa = true;
    printf("Ball launched! Vel: (%.2f, %.2f)\n", velX, velY);
}

void Ball::destruir(b2World* world, std::map<b2Body*, int>& bodyIDs) {
    // Elimina la pelota del mundo Box2D y del mapa de IDs
    if (body != nullptr) {
        bodyIDs.erase(body);
        world->DestroyBody(body);
        body = nullptr;
    }
    activa = false;
}

void Ball::dibujar(float ppm, int screenHeight) {
    // Dibuja la pelota 
    if (body == nullptr || !activa) return;

    b2Vec2 pos = body->GetPosition();
    float screenX = pos.x * ppm;
    float screenY = screenHeight - pos.y * ppm;

    DrawCircle((int)screenX + 3, (int)screenY + 3, 12, ColorAlpha(BLACK, 0.3f));
    DrawCircle((int)screenX, (int)screenY, 12, ORANGE);
    DrawCircleLines((int)screenX, (int)screenY, 12, ColorAlpha(BLACK, 0.4f));
    DrawLine((int)screenX - 12, (int)screenY, (int)screenX + 12, (int)screenY, ColorAlpha(BLACK, 0.3f));
    DrawLine((int)screenX, (int)screenY - 12, (int)screenX, (int)screenY + 12, ColorAlpha(BLACK, 0.3f));
    DrawCircle((int)screenX - 4, (int)screenY - 4, 4, ColorAlpha(WHITE, 0.3f));
}

void Ball::dibujarTrayectoria(float ppm, int screenHeight) {
    // Dibuja puntos de trayectoria predictiva de la pelota
    if (body == nullptr || !activa) return;

    b2Vec2 pos = body->GetPosition();
    b2Vec2 vel = body->GetLinearVelocity();

    float screenX = pos.x * ppm;
    float screenY = screenHeight - pos.y * ppm;

    float mag = sqrt(vel.x * vel.x + vel.y * vel.y);
    if (mag > 0.1f) {
        float dirX = vel.x / mag;
        float dirY = -vel.y / mag;

        for (int i = 1; i <= 20; i++) {
            float alpha = 0.04f + 0.025f * i;
            float size = 3.0f - i * 0.1f;
            if (size < 0.5f) size = 0.5f;
            DrawCircle((int)(screenX + dirX * i * 20),
                (int)(screenY + dirY * i * 20),
                (int)size,
                ColorAlpha(YELLOW, alpha));
        }
    }
}