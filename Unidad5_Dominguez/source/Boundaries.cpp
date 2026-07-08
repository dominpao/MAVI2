#include "Boundaries.h"

Boundaries::Boundaries() : suelo(nullptr), techo(nullptr) {}

Boundaries::~Boundaries() {}

void Boundaries::crear(b2World* world, int screenWidth, int screenHeight, float ppm) {
    b2BodyDef sueloDef;
    sueloDef.position.Set((float)screenWidth / ppm / 2, 0.0f);
    suelo = world->CreateBody(&sueloDef);

    b2PolygonShape sueloShape;
    sueloShape.SetAsBox((float)screenWidth / ppm / 2, 0.5f);
    b2FixtureDef sueloFix;
    sueloFix.shape = &sueloShape;
    sueloFix.friction = 0.3f;
    suelo->CreateFixture(&sueloFix);

    b2BodyDef techoDef;
    techoDef.position.Set((float)screenWidth / ppm / 2, (float)screenHeight / ppm);
    techo = world->CreateBody(&techoDef);

    b2PolygonShape techoShape;
    techoShape.SetAsBox((float)screenWidth / ppm / 2, 0.5f);
    b2FixtureDef techoFix;
    techoFix.shape = &techoShape;
    techoFix.friction = 0.3f;
    techo->CreateFixture(&techoFix);
}