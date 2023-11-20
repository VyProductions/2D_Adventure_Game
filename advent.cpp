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
    int cntX = 32;
    int cntY = 32;

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

        end = std::chrono::high_resolution_clock::now();
        micro_ticks = std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - begin).count();

        if ((double)micro_ticks / 1e6 >= 1 / FPS) {
        //     Update((double)micro_ticks / 1e6);
            begin = std::chrono::high_resolution_clock::now();
            micro_ticks = std::chrono::duration_cast<
                std::chrono::microseconds
            >(begin - last_frame).count();

        //     if (avg_FPS == 0.0) {
        //         avg_FPS = 1e6 / (double)micro_ticks;
        //     } else {
        //         avg_FPS = (avg_FPS + 1e6 / (double)micro_ticks) / 2.0;
        //     }

        //     last_frame = std::chrono::high_resolution_clock::now();
            SDL_RenderClear(renderer);
            // DrawRoundedWindow({10, 10, 256, 128}, 16, {0, 255, 255, 0});
            DrawElipse({10, 10, 32, 64}, NIL, {0, 255, 255, 0});
            DrawElipse({42, 10, 32, 64}, UP, {0, 255, 255, 0});
            DrawElipse({74, 10, 32, 64}, UP_LEFT, {0, 255, 255, 0});
            DrawElipse({106, 10, 32, 64}, UP_RIGHT, {0, 255, 255, 0});
            DrawElipse({138, 10, 32, 64}, LEFT, {0, 255, 255, 0});
            DrawElipse({170, 10, 32, 64}, RIGHT, {0, 255, 255, 0});
            DrawElipse({202, 10, 32, 64}, DOWN_LEFT, {0, 255, 255, 0});
            DrawElipse({234, 10, 32, 64}, DOWN_RIGHT, {0, 255, 255, 0});
            DrawElipse({276, 10, 32, 64}, DOWN, {0, 255, 255, 0});
        //     DrawScreen();
            SDL_RenderPresent(renderer);
        }

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
