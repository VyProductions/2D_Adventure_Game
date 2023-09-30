#include "proto.h"

#define UP      279165
#define DOWN    279166
#define RIGHT   279167
#define LEFT    279168
#define RESIZE  410

extern bool running;      // Whether application should continue or not

using namespace std::chrono_literals;

int main() {
    int ch;  // Keycode value

    sys_start();

    // Game Loop
    while (running) {
        ch = get_ch();

        switch (ch) {
            case RESIZE:
                resized();
                break;
            case ERR:  // No input entered
                break;
            default:  // Unrecognized input, try to process it
                process_input(ch);
                break;
        }

        draw_display();
    }

    sys_exit();
}
