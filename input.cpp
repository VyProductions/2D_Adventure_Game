#include "proto.h"

extern std::unordered_map<
    std::string,  // Program contexts / states
    keybind_t     // Mapping for state (keycode -> action function)
> action_map;

static std::unordered_map<
    SDL_Scancode,
    std::string
> key_map {
    {SDL_SCANCODE_A, "A"}, {SDL_SCANCODE_B, "B"}, {SDL_SCANCODE_C, "C"},
    {SDL_SCANCODE_D, "D"}, {SDL_SCANCODE_E, "E"}, {SDL_SCANCODE_F, "F"},
    {SDL_SCANCODE_G, "G"}, {SDL_SCANCODE_H, "H"}, {SDL_SCANCODE_I, "I"},
    {SDL_SCANCODE_J, "J"}, {SDL_SCANCODE_K, "K"}, {SDL_SCANCODE_L, "L"},
    {SDL_SCANCODE_M, "M"}, {SDL_SCANCODE_N, "N"}, {SDL_SCANCODE_O, "O"},
    {SDL_SCANCODE_P, "P"}, {SDL_SCANCODE_Q, "Q"}, {SDL_SCANCODE_R, "R"},
    {SDL_SCANCODE_S, "S"}, {SDL_SCANCODE_T, "T"}, {SDL_SCANCODE_U, "U"},
    {SDL_SCANCODE_V, "V"}, {SDL_SCANCODE_W, "W"}, {SDL_SCANCODE_X, "X"},
    {SDL_SCANCODE_Y, "Y"}, {SDL_SCANCODE_Z, "Z"}, {SDL_SCANCODE_0, "0"},
    {SDL_SCANCODE_1, "1"}, {SDL_SCANCODE_2, "2"}, {SDL_SCANCODE_3, "3"},
    {SDL_SCANCODE_4, "4"}, {SDL_SCANCODE_5, "5"}, {SDL_SCANCODE_6, "6"},
    {SDL_SCANCODE_7, "7"}, {SDL_SCANCODE_8, "8"}, {SDL_SCANCODE_9, "9"},
    {SDL_SCANCODE_LSHIFT, "LSHIFT"}, {SDL_SCANCODE_RSHIFT, "RSHIFT"},
    {SDL_SCANCODE_LCTRL, "LCTRL"}, {SDL_SCANCODE_RCTRL, "RCTRL"},
    {SDL_SCANCODE_LALT, "LALT"}, {SDL_SCANCODE_RALT, "RALT"}
};

std::string key_name(const SDL_Scancode& k_event) {
    return key_map[k_event];
}

void process_keypress(const SDL_KeyboardEvent& event) {
    extern state_t prog_state;

    const SDL_Scancode& keycode = event.keysym.scancode;

    if (action_map[state_name(prog_state)].size() != 0) {
        if (event.state == SDL_PRESSED) {
            if (
                action_map[state_name(prog_state)][keycode].first != nullptr
            ) {
                action_map[state_name(prog_state)][keycode].first();
            } else {
                log("  No action exists for key " + key_name(keycode) + " DOWN");
            }
        } else if (event.state == SDL_RELEASED) {
            if (
                action_map[state_name(prog_state)][keycode].second != nullptr
            ) {
                action_map[state_name(prog_state)][keycode].second();
            } else {
                log("  No action exists for key " + key_name(keycode) + " UP");
            }
        }
    } else {
        log("  No Key Down binds exist yet for context '" + state_name(prog_state) + '\'');
    }
}
