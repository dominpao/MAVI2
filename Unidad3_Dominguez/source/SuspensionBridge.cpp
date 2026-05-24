#include "SuspensionBridge.h"

SuspensionBridge::SuspensionBridge(b2World* world, float startX, float endX, float surfaceY, float sw, float sh)
    : segWidth(sw), segHeight(sh) {

    const int numSegments = 8;
    b2Vec2 towerLeftPos(startX - 0.5f, surfaceY + 4.0f);
    b2Vec2 towerRightPos(endX + 0.5f, surfaceY + 4.0f);

    b2BodyDef towerDef;
    towerDef.type = b2_staticBody;

    towerDef.position = towerLeftPos;
    towerLeft = world->CreateBody(&towerDef);

    towerDef.position = towerRightPos;
    towerRight = world->CreateBody(&towerDef);

    // 1. Tablas
    for (int i = 0; i < numSegments; i++) {
        float x = startX + i * (endX - startX) / (numSegments - 1);
        b2BodyDef segDef;
        segDef.type = b2_dynamicBody;
        segDef.position.Set(x, surfaceY);
        b2Body* seg = world->CreateBody(&segDef);

        b2PolygonShape shape;
        shape.SetAsBox(segWidth / 2.0f, segHeight / 2.0f);
        b2FixtureDef fix;
        fix.shape = &shape;
        fix.density = 0.4f;
        fix.friction = 0.8f;
        seg->CreateFixture(&fix);

        tables.push_back(seg);
    }

    // Joints de tablas
    for (int i = 0; i < numSegments - 1; i++) {
        b2RevoluteJointDef jDef;
        float jointX = (tables[i]->GetPosition().x + tables[i + 1]->GetPosition().x) / 2.0f;
        jDef.Initialize(tables[i], tables[i + 1], b2Vec2(jointX, surfaceY));
        jDef.collideConnected = false;
        world->CreateJoint(&jDef);
    }

    // 2. Catenaria
    for (int i = 0; i < numSegments; i++) {
        float x = startX + i * (endX - startX) / (numSegments - 1);
        float relX = (x - (startX + endX) / 2.0f) / ((endX - startX) / 2.0f);
        float yCurve = (surfaceY + 1.3f) + (relX * relX) * 1.8f;

        b2BodyDef nodeDef;
        nodeDef.type = b2_dynamicBody;
        nodeDef.position.Set(x, yCurve);
        b2Body* node = world->CreateBody(&nodeDef);

        b2CircleShape circle;
        circle.m_radius = 0.1f;

        b2FixtureDef fix;
        fix.shape = &circle;
        fix.density = 0.2f;
        fix.filter.groupIndex = -1;

        node->CreateFixture(&fix);
        cableNodes.push_back(node);
    }

    // Joints de la catenaria
    for (int i = 0; i < numSegments - 1; i++) {
        b2DistanceJointDef cableJoint;
        cableJoint.Initialize(cableNodes[i], cableNodes[i + 1], cableNodes[i]->GetPosition(), cableNodes[i + 1]->GetPosition());
        cableJoint.collideConnected = false;
        cableJoint.stiffness = 40.0f;
        cableJoint.damping = 2.0f;
        world->CreateJoint(&cableJoint);
    }

    b2DistanceJointDef leftAnchorJoint;
    leftAnchorJoint.Initialize(cableNodes[0], towerLeft, cableNodes[0]->GetPosition(), towerLeftPos);
    leftAnchorJoint.collideConnected = false;
    world->CreateJoint(&leftAnchorJoint);

    b2DistanceJointDef rightAnchorJoint;
    rightAnchorJoint.Initialize(cableNodes.back(), towerRight, cableNodes.back()->GetPosition(), towerRightPos);
    rightAnchorJoint.collideConnected = false;
    world->CreateJoint(&rightAnchorJoint);

    // 3. Péndolas
    for (int i = 0; i < numSegments; i++) {
        b2DistanceJointDef pendola;
        pendola.Initialize(tables[i], cableNodes[i], tables[i]->GetPosition(), cableNodes[i]->GetPosition());
        pendola.collideConnected = false;
        pendola.stiffness = 30.0f;
        pendola.damping = 1.5f;
        world->CreateJoint(&pendola);
    }
}

void SuspensionBridge::Draw(float ppm, Vector2(*toScreen)(b2Vec2)) const {
    if (!towerLeft || !towerRight) return;

    Vector2 scrTowerL = toScreen(towerLeft->GetPosition());
    Vector2 scrTowerR = toScreen(towerRight->GetPosition());
    float groundLevelY = toScreen(b2Vec2(0.0f, 1.0f)).y;

    DrawLineEx(scrTowerL, { scrTowerL.x, groundLevelY }, 12.0f, DARKGRAY);
    DrawLineEx(scrTowerR, { scrTowerR.x, groundLevelY }, 12.0f, DARKGRAY);
    DrawCircleV(scrTowerL, 6.0f, BLACK);
    DrawCircleV(scrTowerR, 6.0f, BLACK);

    for (size_t i = 0; i < tables.size(); i++) {
        DrawLineEx(toScreen(tables[i]->GetPosition()), toScreen(cableNodes[i]->GetPosition()), 1.5f, RAYWHITE);
    }

    Vector2 prevPt = scrTowerL;
    for (size_t i = 0; i < cableNodes.size(); i++) {
        Vector2 currentPt = toScreen(cableNodes[i]->GetPosition());
        DrawLineEx(prevPt, currentPt, 3.5f, BLACK);
        DrawCircleV(currentPt, 3.0f, BLACK);
        prevPt = currentPt;
    }
    DrawLineEx(prevPt, scrTowerR, 3.5f, BLACK);

    for (size_t i = 0; i < tables.size(); i++) {
        b2Body* seg = tables[i];
        Vector2 size = { segWidth * ppm, segHeight * ppm };
        Rectangle rect = { toScreen(seg->GetPosition()).x, toScreen(seg->GetPosition()).y, size.x, size.y };
        DrawRectanglePro(rect, { size.x / 2.0f, size.y / 2.0f }, seg->GetAngle() * RAD2DEG, BROWN);
    }
}