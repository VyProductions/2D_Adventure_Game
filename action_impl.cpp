#include "proto.h"

extern SDL_Renderer* renderer;
extern player_t player;
extern std::unordered_map<
    std::string,
    std::list<object_t>
> map_set;

static bool pressed_arr[5] {};
enum pressed {_UP, _DOWN, _LEFT, _RIGHT, _TURN_LOCK};

std::unordered_map<
    std::string,
    void (*)(void)
> func_map = {
    {
        "ADVANCE_DIALOG", [](void) {
            print("Advancing dialog...");
        }
    }, {
        "EXIT", [](void) {
            extern bool running;
            running = false;
        }
    }, {
        "MOVE_UP", [](void) {
            print("Move up...");

            pressed_arr[_UP] = true;
            dir_t old = player.look_direction;

            if (
                pressed_arr[_DOWN] && (pressed_arr[_LEFT] ^ pressed_arr[_RIGHT])
            ) {
                player.look_direction = pressed_arr[_LEFT] ? LEFT : RIGHT;
            } else {
                player.look_direction = !(pressed_arr[_LEFT] ^ pressed_arr[_RIGHT]) ?
                    pressed_arr[_DOWN] ? DOWN    : UP      :
                    pressed_arr[_LEFT] ? UP_LEFT : UP_RIGHT;
            }

            if (player.look_direction != old) {
                // Reallocate player icon to struct
                SDL_FreeSurface(player.spriteSurface);
                SDL_DestroyTexture(player.spriteTexture);
                player.spriteSurface = SDL_LoadBMP(player.icon().c_str());
                player.spriteTexture = SDL_CreateTextureFromSurface(
                    renderer, player.spriteSurface
                );
            }

            player.velocity.y = pressed_arr[_DOWN] ? 0 : -1;
        }
    }, {
        "MOVE_DOWN", [](void) {
            print("Move down...");

            pressed_arr[_DOWN] = true;
            dir_t old = player.look_direction;

            if (
                pressed_arr[_UP] && (pressed_arr[_LEFT] ^ pressed_arr[_RIGHT])
            ) {
                player.look_direction = pressed_arr[_LEFT] ?
                    LEFT : RIGHT;
            } else {
                player.look_direction = !(pressed_arr[_LEFT] ^ pressed_arr[_RIGHT]) ?
                    pressed_arr[_UP]   ? UP        : DOWN      :
                    pressed_arr[_LEFT] ? DOWN_LEFT : DOWN_RIGHT;
            }

            if (player.look_direction != old) {
                // Reallocate player icon to struct
                SDL_FreeSurface(player.spriteSurface);
                SDL_DestroyTexture(player.spriteTexture);
                player.spriteSurface = SDL_LoadBMP(player.icon().c_str());
                player.spriteTexture = SDL_CreateTextureFromSurface(
                    renderer, player.spriteSurface
                );
            }

            player.velocity.y = pressed_arr[_UP] ? 0 : 1;
        }
    }, {
        "CANCEL_UP", [](void) {
            print("Stop moving up...");

            pressed_arr[_UP] = false;
            dir_t old = player.look_direction;

            if (
                pressed_arr[_DOWN] && (pressed_arr[_LEFT] ^ pressed_arr[_RIGHT])
            ) {
                player.look_direction = pressed_arr[_LEFT] ?
                    DOWN_LEFT : DOWN_RIGHT;
            } else {
                player.look_direction = !(pressed_arr[_LEFT] ^ pressed_arr[_RIGHT]) ?
                    pressed_arr[_DOWN] ? DOWN : UP   :
                    pressed_arr[_LEFT] ? LEFT : RIGHT;
            }

            if (player.look_direction != old) {
                // Reallocate player icon to struct
                SDL_FreeSurface(player.spriteSurface);
                SDL_DestroyTexture(player.spriteTexture);
                player.spriteSurface = SDL_LoadBMP(player.icon().c_str());
                player.spriteTexture = SDL_CreateTextureFromSurface(
                    renderer, player.spriteSurface
                );
            }

            player.velocity.y = pressed_arr[_DOWN] ? 1 : 0;
        }
    }, {
        "CANCEL_DOWN", [](void) {
            print("Stop moving down...");

            pressed_arr[_DOWN] = false;
            dir_t old = player.look_direction;

            if (
                pressed_arr[_UP] && (pressed_arr[_LEFT] ^ pressed_arr[_RIGHT])
            ) {
                player.look_direction = pressed_arr[_LEFT] ?
                    UP_LEFT : UP_RIGHT;
            } else {
                player.look_direction = !(pressed_arr[_LEFT] ^ pressed_arr[_RIGHT]) ?
                    pressed_arr[_UP]   ? UP   : DOWN :
                    pressed_arr[_LEFT] ? LEFT : RIGHT;
            }

            if (player.look_direction != old) {
                // Reallocate player icon to struct
                SDL_FreeSurface(player.spriteSurface);
                SDL_DestroyTexture(player.spriteTexture);
                player.spriteSurface = SDL_LoadBMP(player.icon().c_str());
                player.spriteTexture = SDL_CreateTextureFromSurface(
                    renderer, player.spriteSurface
                );
            }

            player.velocity.y = pressed_arr[_UP] ? -1 : 0;
        }
    }, {
        "MOVE_LEFT", [](void) {
            print("Move left...");
            pressed_arr[_LEFT] = true;

            dir_t old = player.look_direction;

            if (
                pressed_arr[_RIGHT] && (pressed_arr[_UP] ^ pressed_arr[_DOWN])
            ) {
                player.look_direction = pressed_arr[_UP] ? UP : DOWN;
            } else {
                player.look_direction = !(pressed_arr[_UP] ^ pressed_arr[_DOWN]) ?
                    pressed_arr[_RIGHT] ? RIGHT   : LEFT     :
                    pressed_arr[_UP]    ? UP_LEFT : DOWN_LEFT;
            }

            if (player.look_direction != old) {
                // Reallocate player icon to struct
                SDL_FreeSurface(player.spriteSurface);
                SDL_DestroyTexture(player.spriteTexture);
                player.spriteSurface = SDL_LoadBMP(player.icon().c_str());
                player.spriteTexture = SDL_CreateTextureFromSurface(
                    renderer, player.spriteSurface
                );
            }

            player.velocity.x = pressed_arr[_RIGHT] ? 0 : -1;
        }
    }, {
        "MOVE_RIGHT", [](void) {
            print("Move right...");
            pressed_arr[_RIGHT] = true;

            dir_t old = player.look_direction;

            if (
                pressed_arr[_LEFT] && (pressed_arr[_UP] ^ pressed_arr[_DOWN])
            ) {
                player.look_direction = pressed_arr[_UP] ? UP : DOWN;
            } else {
                player.look_direction = !(pressed_arr[_UP] ^ pressed_arr[_DOWN]) ?
                    pressed_arr[_LEFT] ? LEFT     : RIGHT     :
                    pressed_arr[_UP]   ? UP_RIGHT : DOWN_RIGHT;
            }

            if (player.look_direction != old) {
                // Reallocate player icon to struct
                SDL_FreeSurface(player.spriteSurface);
                SDL_DestroyTexture(player.spriteTexture);
                player.spriteSurface = SDL_LoadBMP(player.icon().c_str());
                player.spriteTexture = SDL_CreateTextureFromSurface(
                    renderer, player.spriteSurface
                );
            }

            player.velocity.x = pressed_arr[_LEFT] ? 0 : 1;
        }
    }, {
        "CANCEL_LEFT", [](void) {
            print("Stop moving left...");

            pressed_arr[_LEFT] = false;
            dir_t old = player.look_direction;

            if (
                pressed_arr[_RIGHT] && (pressed_arr[_UP] ^ pressed_arr[_DOWN])
            ) {
                player.look_direction = pressed_arr[_UP] ?
                    UP_RIGHT : DOWN_RIGHT;
            } else {
                player.look_direction = !(pressed_arr[_UP] ^ pressed_arr[_DOWN]) ?
                    pressed_arr[_RIGHT] ? RIGHT : LEFT :
                    pressed_arr[_UP]    ? UP    : DOWN ;
            }

            if (player.look_direction != old) {
                // Reallocate player icon to struct
                SDL_FreeSurface(player.spriteSurface);
                SDL_DestroyTexture(player.spriteTexture);
                player.spriteSurface = SDL_LoadBMP(player.icon().c_str());
                player.spriteTexture = SDL_CreateTextureFromSurface(
                    renderer, player.spriteSurface
                );
            }

            player.velocity.x = pressed_arr[_RIGHT] ? 1 : 0;
        }
    }, {
        "CANCEL_RIGHT", [](void) {
            print("Stop moving right...");

            pressed_arr[_RIGHT] = false;
            dir_t old = player.look_direction;

            if (
                pressed_arr[_LEFT] && (pressed_arr[_UP] ^ pressed_arr[_DOWN])
            ) {
                player.look_direction = pressed_arr[_UP] ?
                    UP_LEFT : DOWN_LEFT;
            } else {
                player.look_direction = !(pressed_arr[_UP] ^ pressed_arr[_DOWN]) ?
                    pressed_arr[_LEFT] ? LEFT : RIGHT :
                    pressed_arr[_UP]   ? UP   : DOWN  ;
            }

            if (player.look_direction != old) {
                // Reallocate player icon to struct
                SDL_FreeSurface(player.spriteSurface);
                SDL_DestroyTexture(player.spriteTexture);
                player.spriteSurface = SDL_LoadBMP(player.icon().c_str());
                player.spriteTexture = SDL_CreateTextureFromSurface(
                    renderer, player.spriteSurface
                );
            }

            player.velocity.x = pressed_arr[_LEFT] ? -1 : 0;
        }
    }, {
        "INTERACT", [](void) {
            print("Interacting...");
        }
    }, {
        "RESPAWN", [](void) {
            log(player.name + " was respawned...");

            player.position = player.spawn_point;
            player.look_direction = !pressed_arr[_UP] && !pressed_arr[_DOWN] &&
                !pressed_arr[_LEFT] && !pressed_arr[_RIGHT] ?
                UP : player.look_direction;
            player.spriteRect.x = (int)(player.position.x + 0.5);
            player.spriteRect.y = (int)(player.position.y + 0.5);
            
            // Reallocate player icon to struct
            SDL_FreeSurface(player.spriteSurface);
            SDL_DestroyTexture(player.spriteTexture);
            player.spriteSurface = SDL_LoadBMP(player.icon().c_str());
            player.spriteTexture = SDL_CreateTextureFromSurface(
                renderer, player.spriteSurface
            );
        }
    }, {
        "PLACE_ORB", [](void) {
            print("Placing orb...");

            // Generate random valid screen position (rounded to int coords)
            vec2_t pos = {
                (long double)(rand() % (int64_t)(WIND_WIDTH - 32)),
                (long double)(rand() % (int64_t)(WIND_HEIGHT - 32))
            };

            // Create object
            object_t orb {
                "Orb", pos, {0, 0}, UP, 0.0, {
                    "BMPs/Red_Orb.bmp",
                    "BMPs/Red_Orb.bmp",
                    "BMPs/Red_Orb.bmp",
                    "BMPs/Red_Orb.bmp",
                    "BMPs/Red_Orb.bmp",
                    "BMPs/Red_Orb.bmp",
                    "BMPs/Red_Orb.bmp",
                    "BMPs/Red_Orb.bmp"
                },
                {(int)pos.x, (int)pos.y, 32, 32},
                {0, 0, 32, 32},
                nullptr, nullptr,
                nullptr, nullptr
            };

            // Allocate sprite surface and texture
            orb.spriteSurface = SDL_LoadBMP(orb.icon().c_str());
            orb.spriteTexture = SDL_CreateTextureFromSurface(
                renderer, orb.spriteSurface
            );

            // Allocate hitbox surface and texture
            orb.hitboxSurface = SDL_CreateRGBSurface(
                0, 32, 32, 32, 0, 0, 0, 0
            );

            SDL_FillRect(orb.hitboxSurface, &orb.hitboxRect, 0x000000FF);

            orb.hitboxTexture = SDL_CreateTextureFromSurface(
                renderer, orb.hitboxSurface
            );

            orb.hitboxRect.x = (int)pos.x;
            orb.hitboxRect.y = (int)pos.y;

            // Add orb to map_set for later deallocation
            map_set[orb.position()].push_back(orb);
        }
    }
};
