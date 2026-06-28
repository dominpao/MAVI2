#include "Platform.h"

Platform::Platform()
    : x(0), y(0), body(nullptr), type(BodyType::PLATFORM)
{
}

Platform::~Platform() {
}

// Init: Crea el cuerpo fisico de la plataforma en Box2D con su colisionador.
// Si es checkpoint agrega un sensor extra
void Platform::Init(b2World* world, float posX, float posY, BodyType bodyType) {
    x = posX;
    y = posY;
    type = bodyType;

    // Cuerpo estatico en la posicion indicada
    b2BodyDef bd;
    bd.position.Set(x / SCALE, y / SCALE);
    body = world->CreateBody(&bd);

    // Colisionador fisico mas angosto que la textura para mejor gameplay
    b2PolygonShape box;
    float halfWidth = (PLAT_W * 0.8f / 2.0f) / SCALE;
    float halfHeight = (PLAT_H * 0.5f / 2.0f) / SCALE;
    box.SetAsBox(halfWidth, halfHeight);

    b2FixtureDef fd;
    fd.shape = &box;
    fd.friction = 0.9f;
    fd.restitution = 0.0f;
    fd.isSensor = false;
    body->CreateFixture(&fd);

    // Si es checkpoint, agregamos un sensor (detecta contacto sin colisionar)
    if (type == BodyType::SENSOR_CHECKPOINT) {
        b2PolygonShape sensorShape;
        float sensorHalfWidth = (PLAT_W * 0.9f / 2.0f) / SCALE;
        float sensorHalfHeight = (PLAT_H * 0.6f / 2.0f) / SCALE;
        sensorShape.SetAsBox(sensorHalfWidth, sensorHalfHeight);

        b2FixtureDef sensorFd;
        sensorFd.shape = &sensorShape;
        sensorFd.isSensor = true;
        sensorFd.friction = 0.0f;
        sensorFd.restitution = 0.0f;
        body->CreateFixture(&sensorFd);
    }

    // Guardamos el tipo en UserData para que ContactListener lo identifique
    UserData* data = new UserData();
    data->type = type;
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
}

float Platform::GetX() const {
    return x;
}

float Platform::GetY() const {
    return y;
}

b2Body* Platform::GetBody() const {
    return body;
}

BodyType Platform::GetType() const {
    return type;
}

bool Platform::IsCheckpoint() const {
    return type == BodyType::SENSOR_CHECKPOINT;
}

bool Platform::IsFinal() const {
    return type == BodyType::FINAL_PLATFORM;
}

void Platform::SetX(float newX) {
    x = newX;
}

// UpdatePositionFromBody: Sincroniza x desde la posicion del cuerpo de Box2D.
//                         Se usa solo para la plataforma final que se mueve con SetTransform
void Platform::UpdatePositionFromBody() {
    if (body) {
        x = body->GetPosition().x * SCALE;
    }
}
