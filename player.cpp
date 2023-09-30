#include "proto.h"

const vec2_t SPAWN_POINT = {0, 0};
player_t player {L"Bob", 100, 0, {0, 0}, SPAWN_POINT, UP};
extern std::unordered_map<
    std::string,
    std::list<object_t>
> map_set;

void respawn() {
    log(player.player_name + L" has died...");

    map_set[player.position()].pop_back();
    player.position = player.spawn_point;
    player.look_direction = UP;
    map_set[player.position()].push_back(
        {"Player", player.icon()}
    );
}
