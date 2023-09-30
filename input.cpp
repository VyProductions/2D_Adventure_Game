#include "proto.h"

extern std::unordered_map<
    std::string,  // Program contexts / states
    keybind_t     // Mapping for state (keycode -> action function)
> action_map;

extern WINDOW* wnd;

int get_ch() {
    static int arr[3] {};

    int result = 0;
    arr[0] = wgetch(wnd);

    switch (*arr) {
        case 27:
            arr[1] = wgetch(wnd);
            arr[2] = wgetch(wnd);
            result = 270000 + 100 * arr[1] + arr[2];
            break;
        default:
            result = arr[0];
            break;
    }

    return result;
}

void process_input(int keycode) {
    extern state_t prog_state;

    if (action_map[state_name(prog_state)].size() != 0) {
        if (action_map[state_name(prog_state)][keycode] != nullptr) {
            action_map[state_name(prog_state)][keycode]();
        } else {
            log(L"  No action exists for keycode " + std::to_wstring(keycode));
        }
    } else {
        log(L"  No keybinds exist yet for context '" + state_wname(prog_state) + L'\'');
    }
}
