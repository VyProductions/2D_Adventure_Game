#include "proto.h"

std::wstring** display;  // Characters on the screen
int r, c;                // Screen coordinate
int nrows, ncols;        // Dimension of display
WINDOW* wnd;             // N-curses window struct
state_t prog_state;      // Current program state (context)
bool running;            // Whether application should continue or not
Console console;         // Output log file interface

static std::unordered_map<
    state_t,
    std::string
> state_name_map = {
    { UNKNOWN, "UNKNOWN" },
    { DIALOG, "DIALOG" },
    { TEXT_PROMPT, "TEXT_PROMPT" },
    { OPTION_PROMPT, "OPTION_PROMPT" },
    { PLAYER_CONTROL, "PLAYER_CONTROL" }
};

static std::unordered_map<
    state_t,
    std::wstring
> state_wname_map = {
    { UNKNOWN, L"UNKNOWN" },
    { DIALOG, L"DIALOG" },
    { TEXT_PROMPT, L"TEXT_PROMPT" },
    { OPTION_PROMPT, L"OPTION_PROMPT" },
    { PLAYER_CONTROL, L"PLAYER_CONTROL" }
};

static std::unordered_map<
    std::string,
    state_t
> name_state_map = {
    { "UNKNOWN", UNKNOWN },
    { "DIALOG", DIALOG },
    { "TEXT_PROMPT", TEXT_PROMPT },
    { "OPTION_PROMPT", OPTION_PROMPT },
    { "PLAYER_CONTROL", PLAYER_CONTROL }
};

extern std::unordered_map<
    std::string,
    std::list<object_t>
> map_set;

extern player_t player;

void sys_start() {
    log(L"[Application Start]");

    read_inputmap();

    prog_state = PLAYER_CONTROL;
    running = true;
    r = c = 0;

    setlocale(LC_ALL, "");
    wnd = initscr();
    cbreak();
    noecho();
    scrollok(wnd, true);
    curs_set(0);
    clear();
    refresh();
    getmaxyx(wnd, nrows, ncols);
    nodelay(wnd, true);

    // Populate map_set with empty object lists
    for (int64_t i = 0; i < nrows; ++i) {
        for (int64_t j = 0; j < ncols; ++j) {
            map_set[vec2_t{j, i}()] = {{"Nil", L" "}};
        }
    }

    // Put player into map
    map_set[player.position()].push_back(
        {"Player", player.icon()}
    );

    log(L"  Populated map...");

    display = new std::wstring*[nrows];

    for (int64_t i = 0; i < nrows; ++i) {
        display[i] = new std::wstring[ncols];

        for (int64_t j = 0; j < ncols; ++j) {
            display[i][j] = !map_set[vec2_t{i, j}()].empty() ?
                map_set[vec2_t{i, j}()].back().icon : L" ";
        }
    }

    log(L"  Allocated display...");

    mvaddwstr(0, 0, L" ");
}

void sys_exit() {
    log(L"[Application End]");

    // Deallocate display array
    for (int i = 0; i < nrows; ++i) {
        delete [] display[i];
    }

    delete [] display;

    endwin();
}

std::string state_name(const state_t& name) {
    return state_name_map[name];
}

std::wstring state_wname(const state_t& name) {
    return state_wname_map[name];
}

state_t name_state(const std::string& name) {
    return (state_t)(
        name_state_map.contains(name) * name_state_map[name]
    );
}

void resized() {
    int rows = nrows;
    int cols = ncols;

    getmaxyx(wnd, nrows, ncols);

    // Reallocate display array
    std::wstring** temp_2d = new std::wstring*[nrows];
    
    for (int i = 0; i < nrows; ++i) {
        temp_2d[i] = new std::wstring[ncols];
    }

    // Copy data from display to temp_2d
    for (int i = 0; i < rows && i < nrows; ++i) {
        for (int j = 0; j < cols && j < ncols; ++j) {
            temp_2d[i][j] = display[i][j];
        }

        // Fill remaining characters in new width with ' '
        for (int j = cols; j < ncols; ++j) {
            temp_2d[i][j] = L" ";
        }
    }

    // Fill remaining rows with ' '
    for (int i = rows; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            temp_2d[i][j] = L" ";
        }
    }

    for (int i = 0; i < rows; ++i) {
        // Free old data row pointer
        delete [] display[i];
    }

    // Free old data root pointer
    delete [] display;

    // Reassign display pointer to new data
    display = temp_2d;

    refresh();
    curs_set(0);
}
