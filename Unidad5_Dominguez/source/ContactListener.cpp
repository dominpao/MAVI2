#include "ContactListener.h"
#include <cstdio>

void ContactListener::BeginContact(b2Contact* contact) {
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();

    int idA = -1, idB = -1;

    if (bodyIDs.find(bodyA) != bodyIDs.end()) {
        idA = bodyIDs[bodyA];
    }
    if (bodyIDs.find(bodyB) != bodyIDs.end()) {
        idB = bodyIDs[bodyB];
    }

    if ((idA == 1 && idB == 2) || (idA == 2 && idB == 1)) {
        pelotaAtajada = true;
        printf("SAVED!\n");
    }

    if ((idA == 1 && idB == 3) || (idA == 3 && idB == 1)) {
        gol = true;
        printf("GOAL!\n");
    }
}