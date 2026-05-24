#ifndef SUSPENSION_BRIDGE_H
#define SUSPENSION_BRIDGE_H

#include <box2d.h>
#include <raylib.h>
#include <vector>

class SuspensionBridge {
private:
    std::vector<b2Body*> tables;
    std::vector<b2Body*> cableNodes;
    b2Body* towerLeft;
    b2Body* towerRight;
    float segWidth;
    float segHeight;

public:
    SuspensionBridge(b2World* world, float startX, float endX, float surfaceY, float sw, float sh);
    void Draw(float ppm, Vector2(*toScreen)(b2Vec2)) const;
};

#endif
