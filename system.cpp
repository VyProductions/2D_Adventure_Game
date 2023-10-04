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
        log(std::string{"Error initializing SDL: "} + SDL_GetError());
        return -1;  // Error
    }

    window = SDL_CreateWindow(
        "2D Adventure Game", 200, 100, 1000, 700, 0
        // SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_INPUT_GRABBED
    );

    renderer = SDL_CreateRenderer(window, -1, 0);

    background = {
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

    return 0;
}

void sys_exit() {
    log("[Application End]");

    // Free player resources
    SDL_FreeSurface(player.spriteSurface);
    SDL_DestroyTexture(player.spriteTexture);

    // Free all object resources
    for (auto& [position, obj_list] : map_set) {
        for (auto& obj : obj_list) {
            SDL_FreeSurface(obj.spriteSurface);
            SDL_DestroyTexture(obj.spriteTexture);
        }
    }

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
    for (auto& [position, obj_list] : map_set) {
        for (auto& obj : obj_list) {
            // Draw individual object
            SDL_RenderCopy(renderer, obj.spriteTexture, NULL, &obj.spriteRect);
        }
    }

    // Draw player
    SDL_RenderCopy(renderer, player.spriteTexture, NULL, &player.spriteRect);

    // Present render data
    SDL_RenderPresent(renderer);
}

bool pt_in_rectX(vec2_t point, const SDL_Rect& rect) {
    return point.x >= rect.x && point.x < rect.x + rect.w;
}

bool pt_in_rectY(vec2_t point, const SDL_Rect& rect) {
    return point.y >= rect.y && point.y < rect.y + rect.h;
}

bool pt_in_rect(vec2_t point, const SDL_Rect& rect) {
    return point.x >= rect.x && point.x < rect.x + rect.w &&
           point.y >= rect.y && point.y < rect.y + rect.h;
}

void Update(double deltaTime) {
    // Update player position based on velocity
    vec2_t new_pos = player.position + player.velocity.normalize() * player.speed * deltaTime;
    SDL_Rect boundary = {0, 0, WIND_WIDTH - 64, WIND_HEIGHT - 64};
    if (
        pt_in_rect(new_pos, boundary)
    ) {
        player.position += player.velocity.normalize() * player.speed * deltaTime;

        player.spriteRect.x = (int)(player.position.x + 0.5);
        player.spriteRect.y = (int)(player.position.y + 0.5);
    } else if (pt_in_rectX(new_pos, boundary)) {
        player.position.x += player.velocity.normalize().x * player.speed * deltaTime;

        player.spriteRect.x = (int)(player.position.x + 0.5);
        player.spriteRect.y = (int)(player.position.y + 0.5);
    } else if (pt_in_rectY(new_pos, boundary)) {
        player.position.y += player.velocity.normalize().y * player.speed * deltaTime;

        player.spriteRect.x = (int)(player.position.x + 0.5);
        player.spriteRect.y = (int)(player.position.y + 0.5);
    }
}