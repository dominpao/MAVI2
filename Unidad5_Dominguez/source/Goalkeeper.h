#ifndef GOALKEEPER_H
#define GOALKEEPER_H

#include <Box2D.h>
#include <raylib.h>
#include <map>

class Goalkeeper {
private:
    b2Body* body;
    Texture2D texture;
    b2Vec2 posicionInicial;

public:
    Goalkeeper();
    ~Goalkeeper();

    void crear(b2World* world, std::map<b2Body*, int>& bodyIDs);
    void update(float ppm, int screenHeight);
    void dibujar(float ppm, int screenHeight);
    void resetPosition();
    void freeze();

    b2Body* getBody() const { return body; }
    b2Vec2 getPosition() const { return body ? body->GetPosition() : b2Vec2(0,0); }
};

#endif