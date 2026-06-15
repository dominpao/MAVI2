#ifndef PLAYER_H
#define PLAYER_H

#include <box2d.h>
#include <raylib.h>

class Renderer;

class Player {
private:
    b2Body* body;
    bool isActivePlayer;
    Color playerColor;
    bool onGround;

public:
    Player(b2World* world, float x, float y, Color color);
    ~Player();

    void moveLeft(float force);
    void moveRight(float force);
    void jump(float impulse);
    void reset(float x, float y);
    void setActive(bool active);
    bool isActive();
    b2Vec2 getPosition();
    b2Body* getBody();
    bool isInMeta(float metaX, float minY, float maxY);
    void draw(Renderer* renderer);
    void updateGroundStatus();
    bool getIsOnGround();
};

#endif