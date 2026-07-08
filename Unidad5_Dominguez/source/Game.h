#ifndef GAME_H
#define GAME_H

#include <Box2D.h>
#include <raylib.h>
#include <string>
#include "GameState.h"
#include "ContactListener.h"
#include "Goalkeeper.h"
#include "Ball.h"
#include "Goal.h"
#include "Boundaries.h"

class Game {
public:
    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGHT = 600;

private:
    static const float PPM;

    b2World* world;
    ContactListener contactListener;
    GameState estado;

    Goalkeeper arquero;
    Ball pelota;
    Goal arco;
    Boundaries limites;

    int goles;
    int atajadas;
    float eventTimer;

public:
    Game();
    ~Game();

    void update();
    void draw();
    bool shouldClose();

private:
    void crearPelota();
    void resetearJuego();
    void drawField();
    void drawUI();
    void drawEventMessage();

    std::string getStateString() const;
    Color getStateColor() const;
};

#endif