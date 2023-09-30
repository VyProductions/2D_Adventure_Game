#include "proto.h"

extern int nrows, ncols;
extern player_t player;
extern std::unordered_map<
    std::string,
    std::list<object_t>
> map_set;

std::unordered_map<
    std::string,
    void (*)(void)
> func_map = {
    {
        "ADVANCE_DIALOG", [](void) {
            log(L"Advancing dialog...");
        }
    }, {
        "EXIT", [](void) {
            extern bool running;
            running = false;
        }
    }, {
        "MOVE_UP", [](void) {
            log(L"Move up...");

            player.look_direction = UP;

            if (player.position.y > 0) {
                map_set[player.position()].pop_back();
                --player.position.y;
                map_set[player.position()].push_back(
                    {"Player", player.icon()}
                );
            }
        }
    }, {
        "TURN_UP", [](void) {
            log(L"Turn up...");

            player.look_direction = UP;
            map_set[player.position()].back().icon = player.icon();
        }
    }, {
        "MOVE_LEFT", [](void) {
            log(L"Move left...");

            player.look_direction = LEFT;

            if (player.position.x > 0) {
                map_set[player.position()].pop_back();
                player.position.x -= 2;
                map_set[player.position()].push_back(
                    {"Player", player.icon()}
                );
            }
        }
    }, {
        "TURN_LEFT", [](void) {
            log(L"Turn left...");

            player.look_direction = LEFT;
            map_set[player.position()].back().icon = player.icon();
        }
    }, {
        "MOVE_DOWN", [](void) {
            log(L"Move down...");

            player.look_direction = DOWN;

            if (player.position.y < nrows - 1) {
                map_set[player.position()].pop_back();
                ++player.position.y;
                map_set[player.position()].push_back(
                    {"Player", player.icon()}
                );
            }
        }
    }, {
        "TURN_DOWN", [](void) {
            log(L"Turn down...");

            player.look_direction = DOWN;
            map_set[player.position()].back().icon = player.icon();
        }
    }, {
        "MOVE_RIGHT", [](void) {
            log(L"Move right...");

            player.look_direction = RIGHT;

            if (player.position.x < ncols - (ncols % 2 == 0 ? 2 : 3)) {
                map_set[player.position()].pop_back();
                player.position.x += 2;
                map_set[player.position()].push_back(
                    {"Player", player.icon()}
                );
            }
        }
    }, {
        "TURN_RIGHT", [](void) {
            log(L"Turn right...");

            player.look_direction = RIGHT;
            map_set[player.position()].back().icon = player.icon();
        }
    }, {
        "INTERACT", [](void) {
            log(L"Interacting...");
        }
    }, {
        "RESPAWN", [](void) {
            respawn();
        }
    }
};
