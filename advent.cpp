#include "proto.h"

#define FPS 30.0

extern bool running;
extern SDL_Renderer* renderer;

int main(int argc, char** argv) {
    if (sys_start() != 0) return 0;

    std::chrono::time_point begin = std::chrono::high_resolution_clock::now();
    std::chrono::time_point end   = std::chrono::high_resolution_clock::now();
    int64_t micro_ticks;

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

        end = std::chrono::high_resolution_clock::now();
        micro_ticks = std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - begin).count();

        if ((double)micro_ticks / 1e6 >= 1 / FPS) {
            Update((double)micro_ticks / 1e6);
            begin = std::chrono::high_resolution_clock::now();
        }

        SDL_RenderClear(renderer);
        DrawScreen();
        SDL_RenderPresent(renderer);
    }

    sys_exit();

    return 0;
}
