#ifndef TYPES_H
#define TYPES_H

#include <algorithm>
#include <chrono>
#include <codecvt>
#include <cstdlib>
#include <curses.h>
#include <fstream>
#include <iostream>
#include <list>
#include <locale.h>
#include <string>
#include <unordered_map>
#include <vector>

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
        std::locale mylocale("");   // get global locale
        logfile.imbue(mylocale);    // Resolve weird issue with wofstream
        origin = std::chrono::high_resolution_clock::now();
    }

    void close() {
        logfile.close();
    }

    template <class T>
    void _log(const T& msg) {
        now = std::chrono::high_resolution_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::microseconds>(now - origin);
        logfile << L"[" << ftick(diff.count()) << L"] " << msg << std::endl;
    }
private:
    std::wstring ftick(int64_t ticks) {
        std::wstring h  = std::to_wstring(ticks / (3'600'000'000));
        std::wstring m  = std::to_wstring((ticks % 3'600'000'000) / 60'000'000);
        std::wstring s  = std::to_wstring((ticks % 60'000'000) / 1'000'000);
        std::wstring us = std::to_wstring((ticks % 1'000'000));
        h = std::wstring(9 - h.length(), '0') + h;
        m = std::wstring(2 - m.length(), '0') + m;
        s = std::wstring(2 - s.length(), '0') + s;
        us = std::wstring(6 - us.length(), '0') + us;

        return h + L':' + m + L':' + s + L'.' + us;
    }

    tick_t origin;
    tick_t now;

    std::wofstream logfile;
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
    int64_t x;  // (-) = left ; (+) = right
    int64_t y;  // (-) = up   ; (+) = down

    bool operator==(const vec2_t rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    std::string operator()() {
        return std::to_string(x) + ',' + std::to_string(y);
    }
};

enum dir_t {
    NIL, UP, LEFT, DOWN, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT
};

struct player_t {
    std::wstring player_name;
    int health;
    int cash;
    vec2_t position;
    vec2_t spawn_point;
    dir_t look_direction;
    std::wstring icon() {
        std::wstring ic;
        switch (look_direction) {
            case UP:         ic = L"\u2191"; break;
            case LEFT:       ic = L"\u2190"; break;
            case DOWN:       ic = L"\u2193"; break;
            case RIGHT:      ic = L"\u2192"; break;
            case UP_LEFT:    ic = L"\u2196"; break;
            case UP_RIGHT:   ic = L"\u2197"; break;
            case DOWN_LEFT:  ic = L"\u2199"; break;
            case DOWN_RIGHT: ic = L"\u2198"; break;
            default:         ic = L"?";      break;
        }

        return ic;
    }
};

struct object_t {
    std::string name;
    std::wstring icon;
};

typedef std::unordered_map<
    int,            // Input keycode value
    void (*)(void)  // Function for that input given the program context
> keybind_t;

#endif
