#ifndef PROTO_H
#define PROTO_H

#include "types.h"

// Macros **********************************************************************

// Console
extern Console console;
#define log(msg) { console._log(msg); }

// Functions *******************************************************************

// System
void sys_start();
void sys_exit();
std::string state_name(const state_t& name);
std::wstring state_wname(const state_t& name);
void resized();

// Input
int get_ch();
void process_input(int keycode);

// Input Map
void read_inputmap();

// Text
void print_msg(const message_t& msg);
std::vector<int> vec_fmt(const std::wstring& str, int fmt_flags = 0);
std::list<std::vector<int> > word_fmt(const std::wstring& str, int fmt_flags = 0);
std::wstring vec_str(std::vector<int> word);

// Player
void respawn();

// Map
void draw_display();

#endif