#ifndef LEVEL_H
#define LEVEL_H

#include <box2d.h>
#include <vector>
#include "Config.h"
#include "Types.h"
#include "ContactListener.h"
#include "Player.h"
#include "Platform.h"

class Level {
public:
    Level();
    ~Level();

    void Init();
    void Step(float dt);
    void RespawnIfFallen();
    bool HasPlayerFallen() const;
    void Reset();

    Player* GetPlayer() const;
    std::vector<Platform*>& GetPlatforms();
    ContactListener* GetContactListener() const;
    b2World* GetWorld() const;
    Platform* GetFinalPlatform() const;

    void StartFinalPlatform();
    bool IsPlatformMoving() const;
    bool IsPlatformFinished() const;

private:
    b2World* world;
    Player* player;
    std::vector<Platform*> platforms;
    ContactListener* contactListener;

    bool platformMoving;
    bool platformFinished;
    float moveDirection;
    b2Vec2 finalPlatformInitialPos;

    void CreatePlatforms();
    void DetectGround();
    void UpdateFinalPlatform();
};

#endif
