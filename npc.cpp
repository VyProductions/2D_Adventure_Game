#include "proto.h"

extern std::unordered_map<
    std::string,
    std::list<object_t>
> map_set;

extern std::unordered_map<
    std::string,
    std::list<npc_t>
> npc_set;

extern SDL_Renderer* renderer;

void spawn_npc(const std::string& name, const vec2_t& pos) {
    npc_t npc = {
        name, pos, {0, 0}, UP, 256.0, {
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
        {0, 0, 64, 64},
        nullptr, nullptr,
        nullptr, nullptr,
        100, false, pos
    };

    npc.spriteSurface = SDL_LoadBMP(npc.icon().c_str());

    npc.spriteTexture = SDL_CreateTextureFromSurface(
        renderer, npc.spriteSurface
    );

    npc.hitboxSurface = SDL_CreateRGBSurface(
        0, 64, 64, 32, 0, 0, 0, 0
    );

    SDL_FillRect(npc.hitboxSurface, &npc.hitboxRect, 0x0000FF00);
    npc.hitboxTexture = SDL_CreateTextureFromSurface(
        renderer, npc.hitboxSurface
    );

    npc.position.x = pos.x;
    npc.position.y = pos.y;
    npc.spriteRect.x = (int)(npc.position.x + 0.5);
    npc.spriteRect.y = (int)(npc.position.y + 0.5);
    npc.hitboxRect.x = (int)(npc.position.x + 0.5);
    npc.hitboxRect.y = (int)(npc.position.y + 0.5);

    npc_set[npc.position()].push_back(npc);
}