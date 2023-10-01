#include "proto.h"

extern SDL_Renderer* renderer;
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
            log("Advancing dialog...");
        }
    }, {
        "EXIT", [](void) {
            extern bool running;
            running = false;
        }
    }, {
        "MOVE_UP", [](void) {
            log("Move up...");

            if (player.look_direction != UP) {
                player.look_direction = UP;

                // Reallocate player icon to struct
                SDL_FreeSurface(player.spriteSurface);
                SDL_DestroyTexture(player.spriteTexture);
                player.spriteSurface = SDL_LoadBMP(player.icon().c_str());
                player.spriteTexture = SDL_CreateTextureFromSurface(
                    renderer, player.spriteSurface
                );
            }

            if (player.position.y > 0) {
                map_set[player.position()].pop_back();
                --player.position.y;
                --player.spriteRect.y;
                map_set[player.position()].push_back(
                    {player.player_name}
                );
            }
        }
    }, {
        "MOVE_LEFT", [](void) {
            log("Move left...");
        }
    }, {
        "MOVE_DOWN", [](void) {
            log("Move down...");

            if (player.look_direction != DOWN) {
                player.look_direction = DOWN;

                // Reallocate player icon to struct
                SDL_FreeSurface(player.spriteSurface);
                SDL_DestroyTexture(player.spriteTexture);
                player.spriteSurface = SDL_LoadBMP(player.icon().c_str());
                player.spriteTexture = SDL_CreateTextureFromSurface(
                    renderer, player.spriteSurface
                );
            }

            if (player.position.y < WIND_HEIGHT - 65) {
                map_set[player.position()].pop_back();
                ++player.position.y;
                ++player.spriteRect.y;
                map_set[player.position()].push_back(
                    {player.player_name}
                );
            }
        }
    }, {
        "MOVE_RIGHT", [](void) {
            log("Move right...");
        }
    }, {
        "INTERACT", [](void) {
            log("Interacting...");
        }
    }, {
        "RESPAWN", [](void) {
            respawn();
        }
    }
};
