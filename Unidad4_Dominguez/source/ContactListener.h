#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include <box2d.h>
#include <iostream>
#include "Types.h"

class ContactListener : public b2ContactListener {
public:
    bool checkpointReached = false;
    bool goalReached = false;
    bool checkpointActive = false;
    bool playerOnFinalPlatform = false;

    void BeginContact(b2Contact* contact) override {
        b2Fixture* fixA = contact->GetFixtureA();
        b2Fixture* fixB = contact->GetFixtureB();

        b2Body* bodyA = fixA->GetBody();
        b2Body* bodyB = fixB->GetBody();

        auto dataA = reinterpret_cast<UserData*>(bodyA->GetUserData().pointer);
        auto dataB = reinterpret_cast<UserData*>(bodyB->GetUserData().pointer);

        if (!dataA || !dataB) return;

        if ((dataA->type == BodyType::PLAYER && dataB->type == BodyType::SENSOR_CHECKPOINT) ||
            (dataB->type == BodyType::PLAYER && dataA->type == BodyType::SENSOR_CHECKPOINT)) {
            checkpointReached = true;
            checkpointActive = true;
            std::cout << "Checkpoint alcanzado!" << std::endl;
        }

        if ((dataA->type == BodyType::PLAYER && dataB->type == BodyType::FINAL_PLATFORM) ||
            (dataB->type == BodyType::PLAYER && dataA->type == BodyType::FINAL_PLATFORM)) {
            goalReached = true;
            playerOnFinalPlatform = true;
            std::cout << "Meta alcanzada!" << std::endl;
        }
    }

    void EndContact(b2Contact* contact) override {
        b2Fixture* fixA = contact->GetFixtureA();
        b2Fixture* fixB = contact->GetFixtureB();

        b2Body* bodyA = fixA->GetBody();
        b2Body* bodyB = fixB->GetBody();

        auto dataA = reinterpret_cast<UserData*>(bodyA->GetUserData().pointer);
        auto dataB = reinterpret_cast<UserData*>(bodyB->GetUserData().pointer);

        if (!dataA || !dataB) return;

        if ((dataA->type == BodyType::PLAYER && dataB->type == BodyType::SENSOR_CHECKPOINT) ||
            (dataB->type == BodyType::PLAYER && dataA->type == BodyType::SENSOR_CHECKPOINT)) {
            checkpointActive = false;
        }

        if ((dataA->type == BodyType::PLAYER && dataB->type == BodyType::FINAL_PLATFORM) ||
            (dataB->type == BodyType::PLAYER && dataA->type == BodyType::FINAL_PLATFORM)) {
            playerOnFinalPlatform = false;
        }
    }
};

#endif
