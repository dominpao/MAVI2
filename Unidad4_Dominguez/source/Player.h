#ifndef PLAYER_H
#define PLAYER_H

#include <box2d.h>
#include "Config.h"
#include "Types.h"

class Player {
public:
    Player();
    ~Player();

    void Init(b2World* world, b2Vec2 spawnPos);
    void MoveRight();
    void MoveLeft();
    void Stop();
    void Jump();
    void CutJump();
    void Respawn();
    void ResetToSpawn();
    void UpdateGroundStatus(bool grounded);
    void UpdateOnPlatform(bool onPlatform);

    b2Body* GetBody() const;
    b2Vec2 GetPosition() const;
    b2Vec2 GetVelocity() const;
    bool IsCheckpointUnlocked() const;
    b2Vec2 GetLastCheckpointPos() const;
    bool IsOnPlatform() const;
    bool IsGrounded() const;

    void SetCheckpointUnlocked(bool unlocked);
    void SetLastCheckpointPos(b2Vec2 pos);
    void SetSpawnPosition(b2Vec2 pos);

private:
    b2Body* body;
    b2Vec2 spawnPosition;
    b2Vec2 lastCheckpointPos;
    bool checkpointUnlocked;
    bool grounded;
    bool onPlatform;
};

#endif
