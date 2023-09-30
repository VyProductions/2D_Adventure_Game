#include "proto.h"

std::unordered_map<
    std::string,  // Program contexts / states
    keybind_t     // Mapping for state (keycode -> action function)
> action_map;

extern std::unordered_map<
    std::string,
    void (*)(void)
> func_map;

void read_inputmap() {
    std::ifstream input("inputs.txt");

    std::string name;
    char ch;
    std::string action_name;

    while (input >> name >> ch >> action_name) {
        if (name == "ANY") {  // Populate all contexts with keybind
            for (
                state_t s = UNKNOWN; s <= PLAYER_CONTROL; s = (state_t)(s + 1)
            ) {
                keybind_t& state_keys = action_map[state_name(s)];

                if (state_keys.empty()) state_keys = {};
                state_keys[(int)ch] = func_map[action_name];
            }
        } else {  // Add keybind to specific context
            keybind_t& state_keys = action_map[name];
            if (state_keys.empty()) state_keys = {};
            state_keys[(int)ch] = func_map[action_name];
        }
    }

    input.close();
}
