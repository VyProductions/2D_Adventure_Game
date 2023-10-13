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

extern std::unordered_map<
    std::string,
    void (*)(void)
> func_map;

extern player_t player;

static std::chrono::time_point begin = std::chrono::high_resolution_clock::now();
static std::chrono::time_point end = std::chrono::high_resolution_clock::now();
static int64_t micro_ticks;

int sys_start() {
    log("[Application Start]");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        log(std::string{"Error initializing SDL: "} + SDL_GetError());
        return -1;  // Error
    }

    if (TTF_Init() != 0) {
        log(std::string{"Error initializing TTF: "} + TTF_GetError());
        return -1;
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

    // Allocate player hitbox
    player.hitboxSurface = SDL_CreateRGBSurface(
        0, 32, 32, 32, 0, 0, 0, 0
    );

    SDL_FillRect(player.hitboxSurface, &player.hitboxRect, 0x00FF0000);
    player.hitboxTexture = SDL_CreateTextureFromSurface(
        renderer, player.hitboxSurface
    );

    player.hitboxRect.x = (int)(player.position.x + 0.5) + 16;
    player.hitboxRect.y = (int)(player.position.y + 0.5) + 16;

    begin = std::chrono::high_resolution_clock::now();

    return 0;
}

void sys_exit() {
    log("[Application End]");

    // Free player resources
    SDL_FreeSurface(player.spriteSurface);
    SDL_DestroyTexture(player.spriteTexture);
    SDL_FreeSurface(player.hitboxSurface);
    SDL_DestroyTexture(player.hitboxTexture);

    // Free all object resources
    for (auto& [position, obj_list] : map_set) {
        for (auto& obj : obj_list) {
            SDL_FreeSurface(obj.spriteSurface);
            SDL_DestroyTexture(obj.spriteTexture);
            SDL_FreeSurface(obj.hitboxSurface);
            SDL_DestroyTexture(obj.hitboxTexture);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
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
            // Draw individual object hitbox
            SDL_RenderCopy(renderer, obj.hitboxTexture, NULL, &obj.hitboxRect);

            // Draw individual object sprite
            SDL_RenderCopy(renderer, obj.spriteTexture, NULL, &obj.spriteRect);
        }
    }

    // Draw player hitbox
    SDL_RenderCopy(renderer, player.hitboxTexture, NULL, &player.hitboxRect);

    // Draw player sprite
    SDL_RenderCopy(renderer, player.spriteTexture, NULL, &player.spriteRect);

    // Draw message
    int font_pt = 24;

    TTF_Font* font = TTF_OpenFont("Fonts/OverpassMono-Regular.ttf", font_pt);

    print_msg(L"Hello, world!", {0, (long double)(WIND_HEIGHT - font_pt)}, font);

    TTF_CloseFont(font);

    // Present render data
    SDL_RenderPresent(renderer);
}

bool inter_lineX(const line_t& lhs, const line_t& rhs) {
    return (lhs.begin.x <= rhs.begin.x && lhs.end.x >= rhs.begin.x) ||
           (lhs.begin.x <= rhs.end.x && lhs.end.x >= rhs.end.x);
}

bool inter_lineY(const line_t& lhs, const line_t& rhs) {
    return (lhs.begin.y <= rhs.begin.y && lhs.end.y >= rhs.begin.y) ||
           (lhs.begin.y <= rhs.end.y && lhs.end.y >= rhs.end.y);
}

bool pt_in_rectX(const vec2_t& point, const SDL_Rect& rect) {
    return point.x >= rect.x && point.x < rect.x + rect.w;
}

bool pt_in_rectY(const vec2_t& point, const SDL_Rect& rect) {
    return point.y >= rect.y && point.y < rect.y + rect.h;
}

bool pt_in_rect(const vec2_t& point, const SDL_Rect& rect) {
    return point.x >= rect.x && point.x < rect.x + rect.w &&
           point.y >= rect.y && point.y < rect.y + rect.h;
}

vec2_t stov(const std::string& pos_str) {
    long double x;
    long double y;

    std::istringstream in(pos_str);

    in.ignore();  // '('
    in >> x;
    in.ignore(2);  // ", "
    in >> y;

    return {x, y};
}

void movePlayer(double deltaTime) {
    // Update player position based on velocity
    vec2_t new_pos = player.position + player.velocity.normalize() * player.speed * deltaTime;
    SDL_Rect boundary = {0, 0, WIND_WIDTH - 64, WIND_HEIGHT - 64};

    if (
        pt_in_rect(new_pos, boundary)
    ) {
        // In bounds entirely

        player.position += player.velocity.normalize() * player.speed * deltaTime;

        player.spriteRect.x = (int)(player.position.x + 0.5);
        player.spriteRect.y = (int)(player.position.y + 0.5);
        player.hitboxRect.x = player.spriteRect.x + 16;
        player.hitboxRect.y = player.spriteRect.y + 16;
    } else if (pt_in_rectX(new_pos, boundary)) {
        // Top or Bottom edge out of bounds

        if (player.velocity.y < 0) {
            player.position.y = 0;
            player.spriteRect.y = 0;
            player.hitboxRect.y = 16;
        } else if (player.velocity.y > 0) {
            player.position.y = WIND_HEIGHT - 64;
            player.spriteRect.y = WIND_HEIGHT - 64;
            player.hitboxRect.y = WIND_HEIGHT - 48;
        }

        player.position.x += player.velocity.normalize().x * player.speed * deltaTime;

        player.spriteRect.x = (int)(player.position.x + 0.5);
        player.hitboxRect.x = player.spriteRect.x + 16;
    } else if (pt_in_rectY(new_pos, boundary)) {
        // Left or Right edge out of bounds

        if (player.velocity.x < 0) {
            player.position.x = 0;
            player.spriteRect.x = 0;
            player.hitboxRect.x = 16;
        } else if (player.velocity.x > 0) {
            player.position.x = WIND_WIDTH - 64;
            player.spriteRect.x = WIND_WIDTH - 64;
            player.hitboxRect.x = WIND_WIDTH - 48;
        }

        player.position.y += player.velocity.normalize().y * player.speed * deltaTime;

        player.spriteRect.y = (int)(player.position.y + 0.5);
        player.hitboxRect.y = player.spriteRect.y + 16;
    } else {
        // Corner out of bounds

        if (player.velocity.x < 0) {
            player.position.x = 0;
            player.spriteRect.x = 0;
            player.hitboxRect.x = 16;
        } else if (player.velocity.x > 0) {
            player.position.x = WIND_WIDTH - 64;
            player.spriteRect.x = WIND_WIDTH - 64;
            player.hitboxRect.x = WIND_WIDTH - 48;
        }

        if (player.velocity.y < 0) {
            player.position.y = 0;
            player.spriteRect.y = 0;
            player.hitboxRect.y = 16;
        } else if (player.velocity.y > 0) {
            player.position.y = WIND_HEIGHT - 64;
            player.spriteRect.y = WIND_HEIGHT - 64;
            player.hitboxRect.y = WIND_HEIGHT - 48;
        }
    }
}

void Update(double deltaTime) {
    end = std::chrono::high_resolution_clock::now();
    micro_ticks = std::chrono::duration_cast<
        std::chrono::microseconds
    >(end - begin).count();

    if ((double)micro_ticks / 1e6 >= 1.0) {
        func_map["PLACE_ORB"]();
        begin = std::chrono::high_resolution_clock::now();
    }

    movePlayer(deltaTime);

    // Detect collision between objects and player
    for (auto& [pos_str, obj_list] : map_set) {
        auto iter = obj_list.begin();
        while (iter != obj_list.end()) {
            const object_t& obj = *iter;
            const vec2_t plr_pos = stov(player.position());
            const vec2_t obj_pos = stov(obj.position());

            // Filter objects based on locality to player position
            //   91 pixel-lengths is maximum intersection distance:
            //     top left of player sprite to bottom right
            if ((plr_pos - obj_pos).magnitude() <= 91) {
                if (
                    inter_lineX(
                        {{plr_pos.x + 16, 0}, {plr_pos.x + 48, 0}},
                        {{obj_pos.x, 0}, {obj_pos.x + 32, 0}}
                    ) &&
                    inter_lineY(
                        {{0, plr_pos.y + 16}, {0, plr_pos.y + 48}},
                        {{0, obj_pos.y}, {0, obj_pos.y + 32}}
                    )
                ) {
                    print(
                        "Player and Object at position " +
                        obj_pos() +
                        " sprites are intersecting."
                    );

                    // Free object resources
                    SDL_FreeSurface(obj.spriteSurface);
                    SDL_DestroyTexture(obj.spriteTexture);
                    SDL_FreeSurface(obj.hitboxSurface);
                    SDL_DestroyTexture(obj.hitboxTexture);

                    // Remove object.
                    iter = obj_list.erase(iter);
                } else {
                    ++iter;
                }
            } else {
                ++iter;
            }
        }
    }
}
