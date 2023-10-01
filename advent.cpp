#include "proto.h"

extern bool running;
extern SDL_Renderer* renderer;

int main(int argc, char** argv) {
    if (sys_start() != 0) return 0;

    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (
                (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) &&
                event.key.repeat == 0  // Not a repeat keypress
            ) {
                process_keypress(event.key);
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                std::cout << '(' << event.button.x << ", " << event.button.y << ") Clicked..." << std::endl;
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                std::cout << '(' << event.button.x << ", " << event.button.y << ") Released..." << std::endl;
            }
        }

        SDL_RenderClear(renderer);
        DrawScreen();
        SDL_RenderPresent(renderer);
    }

    sys_exit();

    return 0;
}
