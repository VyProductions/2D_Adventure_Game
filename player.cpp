#include "proto.h"

const vec2_t SPAWN_POINT = {0, 0};

player_t player;

extern std::unordered_map<
    std::string,
    std::list<object_t>
> map_set;

extern SDL_Renderer* renderer;

void spawn_player(const std::string& name, const vec2_t& pos) {
    if (player.spriteTexture == nullptr) {
        player = { name, SPAWN_POINT, {0, 0}, UP, 256.0, {
                "BMPs/Temp_Player_Up.bmp",
                "BMPs/Temp_Player_Left.bmp",
                "BMPs/Temp_Player_Down.bmp",
                "BMPs/Temp_Player_Right.bmp",
                "BMPs/Temp_Player_UpLeft.bmp",
                "BMPs/Temp_Player_UpRight.bmp",
                "BMPs/Temp_Player_DownLeft.bmp",
                "BMPs/Temp_Player_DownRight.bmp"
            },
            {0, 0, 64, 64},
            {0, 0, 32, 32},
            nullptr, nullptr,
            nullptr, nullptr,
            100, 0, pos
        };

        // Load player BMP into player surface
        player.spriteSurface = SDL_LoadBMP(player.icon().c_str());

        // Create texture from player icon
        player.spriteTexture = SDL_CreateTextureFromSurface(
            renderer, player.spriteSurface
        );

        // Allocate player hitbox
        player.hitboxSurface = SDL_CreateRGBSurface(
            0, 32, 32, 32, 0, 0, 0, 0
        );

        SDL_FillRect(player.hitboxSurface, &player.hitboxRect, 0x00FF0000);
        player.hitboxTexture = SDL_CreateTextureFromSurface(
            renderer, player.hitboxSurface
        );
    }

    player.position.x = pos.x;
    player.position.y = pos.y;
    player.spriteRect.x = (int)(player.position.x + 0.5);
    player.spriteRect.y = (int)(player.position.y + 0.5);
    player.hitboxRect.x = (int)(player.position.x + 0.5) + 16;
    player.hitboxRect.y = (int)(player.position.y + 0.5) + 16;
}