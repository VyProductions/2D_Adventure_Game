#include "proto.h"

std::unordered_map<
    std::string,
    std::list<object_t>
> map_set;

extern std::wstring** display;
extern int r, c;
extern int nrows, ncols;
extern player_t player;

void draw_display() {
    // Draw background characters
    for (int _r = 0; _r < nrows; ++_r) {
        for (int _c = 0; _c < ncols - 1; ++_c) {
            display[_r][_c] = map_set[vec2_t{_c, _r}()].back().icon;
            mvaddwstr(_r, _c, display[_r][_c].c_str());
        }
    }
}
