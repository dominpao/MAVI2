#ifndef BALL_H
#define BALL_H

#include <Box2D.h>
#include <map>

class Ball {
private:
    b2Body* body;
    bool activa;

public:
    Ball();
    ~Ball();

    void crear(b2World* world, std::map<b2Body*, int>& bodyIDs);
    void destruir(b2World* world, std::map<b2Body*, int>& bodyIDs);
    void dibujar(float ppm, int screenHeight);
    void dibujarTrayectoria(float ppm, int screenHeight);

    b2Body* getBody() const { return body; }
    bool estaActiva() const { return activa; }
    void setActiva(bool a) { activa = a; }
};

#endif