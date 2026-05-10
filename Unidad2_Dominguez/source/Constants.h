#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <box2d.h>

const float SCREEN_WIDTH = 1000.0f;
const float SCREEN_HEIGHT = 600.0f;
const b2Vec2 GRAVITY(0.0f, 9.8f);
const float TIME_STEP = 1.0f / 60.0f;
const int VELOCITY_ITERATIONS = 8;
const int POSITION_ITERATIONS = 3;
const float GROUND_HEIGHT = 40.0f;
const float GROUND_Y = SCREEN_HEIGHT - GROUND_HEIGHT;

const float CATAPULT_BASE_X = 180.0f;
const float CATAPULT_BASE_Y = GROUND_Y;
const float ARM_LENGTH = 60.0f;
const float ARM_REST_ANGLE_DEG = 225.0f;
const float ARM_MAX_TENSION_DEG = 180.0f;
const float BASE_IMPULSE = 4500.0f;
const float EXTRA_IMPULSE = 7000.0f;
const float LAUNCH_ANGLE_DEG = 45.0f;

const float PROJECTILE_RADIUS = 15.0f;
const float PROJECTILE_DENSITY = 0.1f;
const float PROJECTILE_FRICTION = 0.4f;
const float PROJECTILE_RESTITUTION = 0.5f;
const float WALL_THICKNESS = 10.0f;
const float CEILING_THICKNESS = 10.0f;

#endif