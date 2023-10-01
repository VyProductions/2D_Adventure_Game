#include "proto.h"

std::unordered_map<  // Keybinds for Key Down events
    std::string,  // Program contexts / states
    keybind_t     // Mapping for state (keycode -> action function)
> action_map;

extern std::unordered_map<
    std::string,
    void (*)(void)
> func_map;

static std::unordered_map<
    std::string,
    SDL_Scancode
> key_scan_map {
    {"A", SDL_SCANCODE_A}, {"B", SDL_SCANCODE_B}, {"C", SDL_SCANCODE_C},
    {"D", SDL_SCANCODE_D}, {"E", SDL_SCANCODE_E}, {"F", SDL_SCANCODE_F},
    {"G", SDL_SCANCODE_G}, {"H", SDL_SCANCODE_H}, {"I", SDL_SCANCODE_I},
    {"J", SDL_SCANCODE_J}, {"K", SDL_SCANCODE_K}, {"L", SDL_SCANCODE_L},
    {"M", SDL_SCANCODE_M}, {"N", SDL_SCANCODE_N}, {"O", SDL_SCANCODE_O},
    {"P", SDL_SCANCODE_P}, {"Q", SDL_SCANCODE_Q}, {"R", SDL_SCANCODE_R},
    {"S", SDL_SCANCODE_S}, {"T", SDL_SCANCODE_T}, {"U", SDL_SCANCODE_U},
    {"V", SDL_SCANCODE_V}, {"W", SDL_SCANCODE_W}, {"X", SDL_SCANCODE_X},
    {"Y", SDL_SCANCODE_Y}, {"Z", SDL_SCANCODE_Z}, {"0", SDL_SCANCODE_0},
    {"1", SDL_SCANCODE_1}, {"2", SDL_SCANCODE_2}, {"3", SDL_SCANCODE_3},
    {"4", SDL_SCANCODE_4}, {"5", SDL_SCANCODE_5}, {"6", SDL_SCANCODE_6},
    {"7", SDL_SCANCODE_7}, {"8", SDL_SCANCODE_8}, {"9", SDL_SCANCODE_9},
    {"LSHIFT", SDL_SCANCODE_LSHIFT}, {"RSHIFT", SDL_SCANCODE_RSHIFT},
    {"LCTRL", SDL_SCANCODE_LCTRL}, {"RCTRL", SDL_SCANCODE_RCTRL},
    {"LALT", SDL_SCANCODE_LALT}, {"RALT", SDL_SCANCODE_RALT}
};

// Alpha characters are from ['A'..'Z'] in the int range [4..29]
// Numerics are from ['1'..'0'] in the int range [30..39]

void read_inputmap() {
    std::ifstream input("inputs.txt");

    std::string name;
    std::string key;
    std::string KD_action_name;
    std::string KU_action_name;

    while (input >> name >> key >> KD_action_name >> KU_action_name) {
        if (name == "ANY") {  // Populate all contexts with keybind
            for (
                state_t s = UNKNOWN; s <= PLAYER_CONTROL; s = (state_t)(s + 1)
            ) {
                keybind_t& state_keys = action_map[state_name(s)];

                if (state_keys.empty()) state_keys = {};

                state_keys[key_scan_map[key]] = {
                    func_map[KD_action_name], func_map[KU_action_name]
                };
            }
        } else {  // Add keybind to specific context
            keybind_t& state_keys = action_map[name];
            if (state_keys.empty()) state_keys = {};
            state_keys[key_scan_map[key]] = {
                func_map[KD_action_name], func_map[KU_action_name]
            };
        }
    }

    input.close();
}
