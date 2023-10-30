#include "proto.h"

#define FPS 30.0

extern bool running;
extern SDL_Renderer* renderer;

int main() {
    if (sys_start() != 0) return 0;

    std::chrono::time_point begin = std::chrono::high_resolution_clock::now();
    std::chrono::time_point end   = std::chrono::high_resolution_clock::now();
    std::chrono::time_point last_frame = std::chrono::high_resolution_clock::now();
    std::chrono::time_point last_report = std::chrono::high_resolution_clock::now();
    int64_t micro_ticks;

    SDL_Event event;

    double avg_FPS = 0.0;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (
                (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) &&
                event.key.repeat == 0  // Not a repeat keypress
            ) {
                process_keypress(event.key);
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                print(
                    std::string{"("} + std::to_string(event.button.x) + ", " +
                    std::to_string(event.button.y) + ") Clicked..."
                );
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                print(
                    std::string{"("} + std::to_string(event.button.x) + ", " +
                    std::to_string(event.button.y) + ") Released..."
                );
            }
        }

        SDL_RenderClear(renderer);
        DrawRoundedWindow({10, 10, 128, 256}, 16, {255, 0, 0, 0});
        // rounded_corner(UP_LEFT, {10, 10, 16, 16}, {255, 0, 0, 0});
        // rounded_corner(UP_RIGHT, {26, 10, 16, 16}, {0, 255, 0, 0});
        // rounded_corner(DOWN_LEFT, {10, 26, 16, 16}, {0, 0, 255, 0});
        // rounded_corner(DOWN_RIGHT, {26, 26, 16, 16}, {255, 255, 0, 0});
        SDL_RenderPresent(renderer);

        // end = std::chrono::high_resolution_clock::now();
        // micro_ticks = std::chrono::duration_cast<
        //     std::chrono::microseconds
        // >(end - begin).count();

        // if ((double)micro_ticks / 1e6 >= 1 / FPS) {
        //     Update((double)micro_ticks / 1e6);
        //     begin = std::chrono::high_resolution_clock::now();
        //     micro_ticks = std::chrono::duration_cast<
        //         std::chrono::microseconds
        //     >(begin - last_frame).count();

        //     if (avg_FPS == 0.0) {
        //         avg_FPS = 1e6 / (double)micro_ticks;
        //     } else {
        //         avg_FPS = (avg_FPS + 1e6 / (double)micro_ticks) / 2.0;
        //     }

        //     last_frame = std::chrono::high_resolution_clock::now();
        //     SDL_RenderClear(renderer);
        //     DrawScreen();
        //     SDL_RenderPresent(renderer);
        // }

        // micro_ticks = std::chrono::duration_cast<
        //     std::chrono::microseconds
        // >(std::chrono::high_resolution_clock::now() - last_report).count();

        // if ((double)micro_ticks / 1e6 >= 0.1) {
        //     last_report = std::chrono::high_resolution_clock::now();

        //     print("Frames per second: " + std::to_string(avg_FPS));
        // }
    }

    sys_exit();

    return 0;
}
