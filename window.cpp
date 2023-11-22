#include "proto.h"

extern SDL_Renderer* renderer;

std::unordered_map<
    std::string,
    std::list<window_t>
> window_set;

/// @brief Draws a rectangular window with circular corners of radius 'radius'
/// @param bounds : the rectangular boundary of the window
/// @param radius : the radius in pixels for the corners of the window
/// @param color  : what color should the window be drawn in
void DrawRoundedWindow(SDL_Rect bounds, int radius, SDL_Color color) {
    const int& X = bounds.x;  // Horizontal pixel offset of window
    const int& Y = bounds.y;  // Vertical pixel offset of window
    const int& H = bounds.h;  // Pixel height of window
    const int& W = bounds.w;  // Pixel width of window
    const int& R = radius;    // Corner radius reference

    #define VERT_WIND(lhs, rhs) {{lhs, rhs}, color, {1,1}}
    #define ELL_PARAM(lhs, rhs, dir) {lhs, rhs, 2*R, 2*R}, dir, color

    // Draw corners
    DrawEllipse(ELL_PARAM(X, Y, UP_LEFT));                             // TL
    DrawEllipse(ELL_PARAM(X + W - R + R, Y, UP_RIGHT));                // TR
    DrawEllipse(ELL_PARAM(X, Y + H - R + R, DOWN_LEFT));               // BL
    DrawEllipse(ELL_PARAM(X + W - R + R, Y + H - R + R, DOWN_RIGHT));  // BR

    // Fill in gaps with rectangles
    SDL_Vertex verts[12] {
        // Left Rectangle
        VERT_WIND(X, Y + R),              // TL
        VERT_WIND(X, Y + H - R),          // BL
        VERT_WIND(X + R, Y + R),          // TR
        VERT_WIND(X + R, Y + H - R),      // BR

        // Middle Rectangle
        VERT_WIND(X + R, Y),              // TL
        VERT_WIND(X + R, Y + H),          // BL
        VERT_WIND(X + W - R, Y),          // TR
        VERT_WIND(X + W - R, Y + H),      // BR

        // Right Rectangle
        VERT_WIND(X + W - R, Y + R),      // TL
        VERT_WIND(X + W - R, Y + H - R),  // BL
        VERT_WIND(X + W, Y + R),          // TR
        VERT_WIND(X + W, Y + H - R)       // BR
    };

    int tris[18] = {
        0, 1, 2,
        1, 2, 3,
        4, 5, 6,
        5, 6, 7,
        8, 9, 10,
        9, 10, 11
    };

    SDL_RenderGeometry(renderer, NULL, verts, 12, tris, 18);
}
