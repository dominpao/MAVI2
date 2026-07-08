#ifndef BOUNDARIES_H
#define BOUNDARIES_H

#include <Box2D.h>

class Boundaries {
private:
    b2Body* suelo;
    b2Body* techo;

public:
    Boundaries();
    ~Boundaries();

    void crear(b2World* world, int screenWidth, int screenHeight, float ppm);
};

#endif