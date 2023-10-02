#include "proto.h"

const vec2_t SPAWN_POINT = {0, 0};

player_t player {"Bob", 100, 0, {0, 0}, {0, 0}, SPAWN_POINT, UP, {
        "BMPs/Temp_Player_Up.bmp",
        "BMPs/Temp_Player_Left.bmp",
        "BMPs/Temp_Player_Down.bmp",
        "BMPs/Temp_Player_Right.bmp",
        "BMPs/Temp_Player_UpLeft.bmp",
        "BMPs/Temp_Player_UpRight.bmp",
        "BMPs/Temp_Player_DownLeft.bmp",
        "BMPs/Temp_Player_DownRight.bmp"
    }, {0, 0, 64, 64}  // x, y, w, h
};

extern std::unordered_map<
    std::string,
    std::list<object_t>
> map_set;

void respawn() {
    log(player.player_name + " has died...");

    map_set[player.position()].pop_back();
    player.position = player.spawn_point;
    player.look_direction = UP;
    map_set[player.position()].push_back(
        {"Player"}
    );
}
