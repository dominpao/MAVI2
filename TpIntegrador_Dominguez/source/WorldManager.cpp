#include "WorldManager.h"
#include "Constants.h"

WorldManager::WorldManager() :
    world(nullptr), movingPlatformJoint(nullptr), brazo(nullptr),
    centralPlat(nullptr), metaPlat(nullptr), platMov(nullptr), pendulumPlatform(nullptr) {
    world = new b2World(b2Vec2(0.0f, GRAVITY));
    createGround();
    createCentralPlatform();
    createCrane();
    createPendulum();
    createMovingPlatform();
    createGoalPlatform();
}

WorldManager::~WorldManager() { delete world; }

void WorldManager::createGround() {
    b2BodyDef gDef;
    gDef.position.Set(15.0f, 0.5f);
    b2Body* ground = world->CreateBody(&gDef);
    b2PolygonShape gBox;
    gBox.SetAsBox(35.0f, 0.5f);
    ground->CreateFixture(&gBox, 0.0f);
    staticBodies.push_back(ground);
}

void WorldManager::createCentralPlatform() {
    b2BodyDef platDef;
    platDef.position.Set(12.0f, 2.0f);
    centralPlat = world->CreateBody(&platDef);
    b2PolygonShape platShape;
    platShape.SetAsBox(3.0f, 0.25f);
    b2FixtureDef platFix;
    platFix.shape = &platShape;
    platFix.friction = 0.6f;
    centralPlat->CreateFixture(&platFix);
    staticBodies.push_back(centralPlat);
}

void WorldManager::createCrane() {
    // Mastil (sin colision para no trepar)
    b2BodyDef mDef;
    mDef.type = b2_staticBody;
    mDef.position.Set(2.0f, 8.0f);
    b2Body* mastil = world->CreateBody(&mDef);
    b2PolygonShape mBox;
    mBox.SetAsBox(0.2f, 8.0f);
    b2FixtureDef mFix;
    mFix.shape = &mBox;
    mFix.isSensor = true;
    mastil->CreateFixture(&mFix);
    staticBodies.push_back(mastil);

    // Brazo de la grua
    b2BodyDef bDef;
    bDef.type = b2_staticBody;
    bDef.position.Set(7.0f, 12.0f);
    brazo = world->CreateBody(&bDef);
    b2PolygonShape bBox;
    bBox.SetAsBox(5.0f, 0.15f);
    brazo->CreateFixture(&bBox, 0.0f);
    staticBodies.push_back(brazo);
}

// Pendulo con RevoluteJoint
void WorldManager::createPendulum() {
    b2BodyDef pDef;
    pDef.type = b2_dynamicBody;
    pDef.position.Set(7.0f, 5.0f);
    pDef.fixedRotation = false;
    pendulumPlatform = world->CreateBody(&pDef);

    b2PolygonShape pBox;
    pBox.SetAsBox(1.2f, 0.1f);
    b2FixtureDef pFixt;
    pFixt.shape = &pBox;
    pFixt.density = 15.0f;
    pFixt.friction = 0.5f;
    pendulumPlatform->CreateFixture(&pFixt);

    b2RevoluteJointDef jointDef;
    jointDef.Initialize(brazo, pendulumPlatform, b2Vec2(7.0f, 12.0f));
    jointDef.enableMotor = false;
    jointDef.lowerAngle = -1.0f;
    jointDef.upperAngle = 1.0f;
    jointDef.enableLimit = true;
    world->CreateJoint(&jointDef);

    pendulumPlatform->ApplyLinearImpulseToCenter(b2Vec2(3.5f, 0), true);
}

// Plataforma movil con PrismaticJoint
void WorldManager::createMovingPlatform() {
    b2BodyDef cDef;
    cDef.type = b2_staticBody;
    cDef.position.Set(19.0f, 7.0f);
    b2Body* carril = world->CreateBody(&cDef);

    b2BodyDef pmDef;
    pmDef.type = b2_dynamicBody;
    pmDef.position.Set(17.0f, 7.0f);
    platMov = world->CreateBody(&pmDef);

    b2PolygonShape pmBox;
    pmBox.SetAsBox(2.0f, 0.1f);
    b2FixtureDef pmFix;
    pmFix.shape = &pmBox;
    pmFix.density = 1.0f;
    pmFix.friction = 0.5f;
    platMov->CreateFixture(&pmFix);

    b2PrismaticJointDef prismDef;
    prismDef.Initialize(carril, platMov, cDef.position, b2Vec2(1, 0));
    prismDef.enableMotor = true;
    prismDef.motorSpeed = 1.5f;
    prismDef.maxMotorForce = 150.0f;
    prismDef.lowerTranslation = -3.0f;
    prismDef.upperTranslation = 3.0f;
    prismDef.enableLimit = true;
    movingPlatformJoint = (b2PrismaticJoint*)world->CreateJoint(&prismDef);
}

void WorldManager::createGoalPlatform() {
    b2BodyDef metaDef;
    metaDef.position.Set(26.0f, 9.0f);
    metaPlat = world->CreateBody(&metaDef);
    b2PolygonShape metaShape;
    metaShape.SetAsBox(1.5f, 0.25f);
    metaPlat->CreateFixture(&metaShape, 0.0f);
    staticBodies.push_back(metaPlat);
}

void WorldManager::step(float timeStep, int velocityIterations, int positionIterations) {
    world->Step(timeStep, velocityIterations, positionIterations);
}

b2World* WorldManager::getWorld() { return world; }

void WorldManager::updateMovingPlatform() {
    if (movingPlatformJoint) {
        float x = movingPlatformJoint->GetJointTranslation();
        if (x >= 2.8f) movingPlatformJoint->SetMotorSpeed(-1.5f);
        else if (x <= -2.8f) movingPlatformJoint->SetMotorSpeed(1.5f);
    }
}

void WorldManager::constrainPlayers(b2Body* player1, b2Body* player2) {
    for (b2Body* player : { player1, player2 }) {
        b2Vec2 pos = player->GetPosition();
        if (pos.x < 0.5f) player->SetTransform(b2Vec2(0.5f, pos.y), 0);
        if (pos.x > 30.5f) player->SetTransform(b2Vec2(30.5f, pos.y), 0);
        if (pos.y < 0.5f) player->SetTransform(b2Vec2(12.0f, 2.2f), 0);
    }
}

b2PrismaticJoint* WorldManager::getMovingPlatformJoint() { return movingPlatformJoint; }
b2Body* WorldManager::getCentralPlatform() { return centralPlat; }
b2Body* WorldManager::getBrazo() { return brazo; }
b2Body* WorldManager::getMetaPlat() { return metaPlat; }
b2Body* WorldManager::getPlatMov() { return platMov; }
b2Body* WorldManager::getPendulumPlatform() { return pendulumPlatform; }