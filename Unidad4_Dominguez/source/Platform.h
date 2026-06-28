#ifndef PLATFORM_H
#define PLATFORM_H

#include <box2d.h>
#include "Config.h"
#include "Types.h"

class Platform {
public:
    Platform();
    ~Platform();

    void Init(b2World* world, float x, float y, BodyType type);
    float GetX() const;
    float GetY() const;
    b2Body* GetBody() const;
    BodyType GetType() const;
    bool IsCheckpoint() const;
    bool IsFinal() const;
    void SetX(float x);
    void UpdatePositionFromBody();

private:
    float x, y;
    b2Body* body;
    BodyType type;
};

#endif
