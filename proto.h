#ifndef PROTO_H
#define PROTO_H

#include "types.h"

// Macros **********************************************************************

// Console
extern Console console;
#define log(msg) { console._log(msg, true); }
#define print(msg) { console._log(msg, false); }

// Functions *******************************************************************

// System
int sys_start();
void sys_exit();
std::string state_name(const state_t& name);
void resized();
void DrawScreen();
bool inter_lineX(const line_t& lhs, const line_t& rhs);
bool inter_lineY(const line_t& lhs, const line_t& rhs);
bool pt_in_rect(const vec2_t& point, const SDL_Rect& rect);
bool pt_in_rectX(const vec2_t& point, const SDL_Rect& rect);
bool pt_in_rectY(const vec2_t& point, const SDL_Rect& rect);
vec2_t stov(const std::string& pos_str);
void movePlayer(double deltaTime);
void Update(double deltaTime);
void DrawEllipse(SDL_Rect bound, dir_t dir, SDL_Color color);
void selectable_press(const vec2_t& mouse_pos);
void selectable_release(const vec2_t& mouse_pos);

// Input
void process_keypress(const SDL_KeyboardEvent& event);

// Input Map
void read_inputmap();

// Text
void print_msg(
    const std::wstring& msg, SDL_Rect bound, TTF_Font* font,
    SDL_Color text_color
);

// Player
void spawn_player(const std::string& name, const vec2_t& pos);

// NPC
void spawn_npc(const std::string& name, const vec2_t& pos);

// Window
void DrawRoundedWindow(SDL_Rect bounds, int radius, SDL_Color color);

#endif