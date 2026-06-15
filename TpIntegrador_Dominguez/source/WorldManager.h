#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include <box2d.h>
#include <vector>

class WorldManager {
private:
    b2World* world;
    std::vector<b2Body*> staticBodies;
    b2PrismaticJoint* movingPlatformJoint;
    b2Body* brazo;
    b2Body* centralPlat;
    b2Body* metaPlat;
    b2Body* platMov;
    b2Body* pendulumPlatform;  

    void createGround();
    void createCentralPlatform();
    void createCrane();
    void createPendulum();
    void createMovingPlatform();
    void createGoalPlatform();

public:
    WorldManager();
    ~WorldManager();

    void step(float timeStep, int velocityIterations, int positionIterations);
    b2World* getWorld();
    void updateMovingPlatform();
    void constrainPlayers(b2Body* player1, b2Body* player2);
    b2PrismaticJoint* getMovingPlatformJoint();
    b2Body* getCentralPlatform();
    b2Body* getBrazo();
    b2Body* getMetaPlat();
    b2Body* getPlatMov();
    b2Body* getPendulumPlatform();  
};

#endif