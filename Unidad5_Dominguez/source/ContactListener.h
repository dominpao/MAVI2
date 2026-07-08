#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include <Box2D.h>
#include <map>

class ContactListener : public b2ContactListener {
public:
    std::map<b2Body*, int> bodyIDs;

    bool pelotaAtajada = false;
    bool gol = false;

    void BeginContact(b2Contact* contact) override;
};

#endif