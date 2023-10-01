#include "proto.h"

SDL_Window* window;      // SDL2 Window reference
SDL_Renderer* renderer;  // SDL2 Renderer reference
SDL_Rect background;     // Rectange bounding entire background of window
state_t prog_state;      // Current program state (context)
bool running;            // Whether application should continue or not
Console console;         // Output log file interface

static std::unordered_map<
    state_t,
    std::string
> state_name_map = {
    { UNKNOWN, "UNKNOWN" },
    { DIALOG, "DIALOG" },
    { TEXT_PROMPT, "TEXT_PROMPT" },
    { OPTION_PROMPT, "OPTION_PROMPT" },
    { PLAYER_CONTROL, "PLAYER_CONTROL" }
};

static std::unordered_map<
    state_t,
    std::string
> state_wname_map = {
    { UNKNOWN, "UNKNOWN" },
    { DIALOG, "DIALOG" },
    { TEXT_PROMPT, "TEXT_PROMPT" },
    { OPTION_PROMPT, "OPTION_PROMPT" },
    { PLAYER_CONTROL, "PLAYER_CONTROL" }
};

static std::unordered_map<
    std::string,
    state_t
> name_state_map = {
    { "UNKNOWN", UNKNOWN },
    { "DIALOG", DIALOG },
    { "TEXT_PROMPT", TEXT_PROMPT },
    { "OPTION_PROMPT", OPTION_PROMPT },
    { "PLAYER_CONTROL", PLAYER_CONTROL }
};

extern std::unordered_map<
    std::string,
    std::list<object_t>
> map_set;

extern player_t player;

int sys_start() {
    log("[Application Start]");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return -1;  // Error
    }

    window = SDL_CreateWindow(
        "2D Adventure Game", 0, 0, 1000, 700,
        SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_INPUT_GRABBED
    );

    renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Rect background = {
        0,           // top left x
        0,           // top left y
        WIND_WIDTH,  // rect width
        WIND_HEIGHT  // rect height
    };

    read_inputmap();

    prog_state = PLAYER_CONTROL;
    running = true;

    // Load player BMP into player surface
    player.spriteSurface = SDL_LoadBMP(player.icon().c_str());

    // Create texture from player icon
    player.spriteTexture = SDL_CreateTextureFromSurface(
        renderer, player.spriteSurface
    );

    // Put player into map
    map_set[player.position()].push_back(
        {"Player"}
    );

    // Render player sprite
    SDL_RenderCopy(renderer, player.spriteTexture, NULL, &player.spriteRect);
    SDL_RenderPresent(renderer);

    return 0;
}

void sys_exit() {
    log("[Application End]");

    // Free player resources
    SDL_FreeSurface(player.spriteSurface);
    SDL_DestroyTexture(player.spriteTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

std::string state_name(const state_t& name) {
    return state_name_map[name];
}

state_t name_state(const std::string& name) {
    return (state_t)(
        name_state_map.contains(name) * name_state_map[name]
    );
}

void resized() {
}

void DrawScreen() {
    // Draw background elements

    // Draw player
    SDL_RenderCopy(renderer, player.spriteTexture, NULL, &player.spriteRect);
    SDL_RenderPresent(renderer);
}
