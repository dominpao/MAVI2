#ifndef GOAL_H
#define GOAL_H

#include <Box2D.h>
#include <map>

class Goal {
private:
    b2Body* body;

public:
    Goal();
    ~Goal();

    void crear(b2World* world, std::map<b2Body*, int>& bodyIDs);
    void dibujar(float ppm, int screenHeight, bool pelotaActiva, bool running);

    b2Body* getBody() const { return body; }
};

#endif