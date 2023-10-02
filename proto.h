#ifndef PROTO_H
#define PROTO_H

#include "types.h"

// Macros **********************************************************************

// Console
extern Console console;
#define log(msg) { console._log(msg); }

// Functions *******************************************************************

// System
int sys_start();
void sys_exit();
std::string state_name(const state_t& name);
void resized();
void DrawScreen();
void Update(double deltaTime);

// Input
void process_keypress(const SDL_KeyboardEvent& event);

// Input Map
void read_inputmap();

// Text
void print_msg(const message_t& msg);

// Player
void respawn();

#endif