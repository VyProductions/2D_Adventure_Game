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

            player.velocity.y = -1;
        }
    }, {
        "CANCEL_UP", [](void) {
            log("Stop moving up...");
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

            player.velocity.y = 1;
        }
    }, {
        "CANCEL_DOWN", [](void) {
            log("Stop moving down...");
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
