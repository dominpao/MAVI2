#ifndef TYPES_H
#define TYPES_H

enum class BodyType { PLAYER, PLATFORM, SENSOR_CHECKPOINT, FINAL_PLATFORM };
enum class GameState { WAITING, RUNNING, CHECKPOINT_REACHED, FINISHED };

struct UserData {
    BodyType type;
};

#endif
