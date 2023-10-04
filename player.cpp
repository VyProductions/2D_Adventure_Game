#include "proto.h"

const vec2_t SPAWN_POINT = {0, 0};

player_t player { "Bob", SPAWN_POINT, {0, 0}, UP, 256.0, {
        "BMPs/Temp_Player_Up.bmp",
        "BMPs/Temp_Player_Left.bmp",
        "BMPs/Temp_Player_Down.bmp",
        "BMPs/Temp_Player_Right.bmp",
        "BMPs/Temp_Player_UpLeft.bmp",
        "BMPs/Temp_Player_UpRight.bmp",
        "BMPs/Temp_Player_DownLeft.bmp",
        "BMPs/Temp_Player_DownRight.bmp"
    }, {0, 0, 64, 64}, nullptr, nullptr, 100, 0, SPAWN_POINT
};

extern std::unordered_map<
    std::string,
    std::list<object_t>
> map_set;
