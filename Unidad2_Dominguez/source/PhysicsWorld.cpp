#include "PhysicsWorld.h"
#include "Constants.h"

PhysicsWorld::PhysicsWorld() : world(GRAVITY) {
    createGround();
    createWalls();
}

PhysicsWorld::~PhysicsWorld() {}

void PhysicsWorld::step() {
    world.Step(TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
}

void PhysicsWorld::createGround() {
    b2BodyDef groundDef;
    groundDef.type = b2_staticBody;
    groundDef.position.Set(SCREEN_WIDTH / 2.0f, GROUND_Y);
    b2Body* groundBody = world.CreateBody(&groundDef);
    b2PolygonShape groundShape;
    groundShape.SetAsBox(SCREEN_WIDTH / 2.0f, GROUND_HEIGHT / 2.0f);
    groundBody->CreateFixture(&groundShape, 0.0f);
}

void PhysicsWorld::createWalls() {
    b2BodyDef leftDef;
    leftDef.type = b2_staticBody;
    leftDef.position.Set(0.0f, SCREEN_HEIGHT / 2.0f);
    b2Body* leftWall = world.CreateBody(&leftDef);
    b2PolygonShape leftShape;
    leftShape.SetAsBox(WALL_THICKNESS / 2.0f, SCREEN_HEIGHT / 2.0f);
    leftWall->CreateFixture(&leftShape, 0.0f);

    b2BodyDef rightDef;
    rightDef.type = b2_staticBody;
    rightDef.position.Set(SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f);
    b2Body* rightWall = world.CreateBody(&rightDef);
    b2PolygonShape rightShape;
    rightShape.SetAsBox(WALL_THICKNESS / 2.0f, SCREEN_HEIGHT / 2.0f);
    rightWall->CreateFixture(&rightShape, 0.0f);

    b2BodyDef ceilingDef;
    ceilingDef.type = b2_staticBody;
    ceilingDef.position.Set(SCREEN_WIDTH / 2.0f, 0.0f);
    b2Body* ceiling = world.CreateBody(&ceilingDef);
    b2PolygonShape ceilingShape;
    ceilingShape.SetAsBox(SCREEN_WIDTH / 2.0f, CEILING_THICKNESS / 2.0f);
    ceiling->CreateFixture(&ceilingShape, 0.0f);
}