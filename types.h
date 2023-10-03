#ifndef TYPES_H
#define TYPES_H

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <codecvt>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <locale.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "SDL2/include/SDL2/SDL.h"
#include "SDL2/include/SDL2/SDL_ttf.h"

enum state_t {
    UNKNOWN, DIALOG, TEXT_PROMPT, OPTION_PROMPT, PLAYER_CONTROL
};

class Console {
public:
    using tick_t = std::chrono::time_point<std::chrono::high_resolution_clock>;

    Console() { open(); }

    ~Console() { close(); }

    void open() {
        logfile.open("log.txt", std::ios::out | std::ios::trunc);
        origin = std::chrono::high_resolution_clock::now();
    }

    void close() {
        logfile.close();
    }

    template <class T>
    void _log(const T& msg) {
        now = std::chrono::high_resolution_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::microseconds>(now - origin);
        // logfile << "[" << ftick(diff.count()) << "] " << msg << std::endl;
        std::cout << "[" << ftick(diff.count()) << "] " << msg << std::endl;
    }
private:
    std::string ftick(int64_t ticks) {
        std::string h  = std::to_string(ticks / (3'600'000'000));
        std::string m  = std::to_string((ticks % 3'600'000'000) / 60'000'000);
        std::string s  = std::to_string((ticks % 60'000'000) / 1'000'000);
        std::string us = std::to_string((ticks % 1'000'000));
        h = std::string(9 - h.length(), '0') + h;
        m = std::string(2 - m.length(), '0') + m;
        s = std::string(2 - s.length(), '0') + s;
        us = std::string(6 - us.length(), '0') + us;

        return h + ':' + m + ':' + s + '.' + us;
    }

    tick_t origin;
    tick_t now;

    std::ofstream logfile;
};

struct message_t {
    // for each int... M.S. Bytes : L.S. Byte -> format : character
    std::vector<int> header;

    // list of words in the content of the message. Same fmt options as header
    std::list<std::vector<int> > content;

    // list of selectable action prompts associated with message
    // - each option is separated further into a list of words. Same fmt options
    //   as header
    std::list<std::list<std::vector<int> > > options;
};

struct vec2_t {
    long double x;  // (-) = left ; (+) = right
    long double y;  // (-) = up   ; (+) = down

    bool operator==(const vec2_t rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    std::string operator()() {
        return std::to_string(x) + ',' + std::to_string(y);
    }

    vec2_t operator*(double multi) {

        return {x * multi, y * multi};
    }

    vec2_t operator+(const vec2_t& rhs) {
        return {x + rhs.x, y + rhs.y};
    }

    vec2_t& operator+=(const vec2_t& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    vec2_t normalize() {
        return {
            x / sqrt(x * x + y * y),
            y / sqrt(x * x + y * y)
        };
    }
};

enum dir_t {
    NIL, UP, LEFT, DOWN, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT
};

struct player_t {
    std::string player_name;
    int health;
    int cash;
    vec2_t position;
    vec2_t velocity;
    vec2_t spawn_point;
    dir_t look_direction;
    double speed;

    std::string icon() {
        std::string ic;

        switch (look_direction) {
            case UP:         ic = icons.at(0); break;
            case LEFT:       ic = icons.at(1); break;
            case DOWN:       ic = icons.at(2); break;
            case RIGHT:      ic = icons.at(3); break;
            case UP_LEFT:    ic = icons.at(4); break;
            case UP_RIGHT:   ic = icons.at(5); break;
            case DOWN_LEFT:  ic = icons.at(6); break;
            case DOWN_RIGHT: ic = icons.at(7); break;
            default:         ic = "";      break;
        }

        return ic;
    }

    vec2_t _position() {
        return {(int)(position.x + 0.5), (int)(position.y + 0.5)};
    }

    std::array<std::string, 8> icons;
    SDL_Rect spriteRect;
    SDL_Surface* spriteSurface;
    SDL_Texture* spriteTexture;
};

struct object_t {
    std::string name;
};

typedef std::unordered_map<
    // Input key event
    SDL_Scancode,
    // Function for that input given the program context
    std::pair<void (*)(void), void (*)(void)>
> keybind_t;

#define WIND_WIDTH 1000
#define WIND_HEIGHT 700

#endif
