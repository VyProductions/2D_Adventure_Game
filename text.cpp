#include "proto.h"

extern SDL_Renderer* renderer;

void print_msg(const std::wstring& msg, vec2_t pos, TTF_Font* font) {
    SDL_Surface* text_box = TTF_RenderUNICODE_Blended_Wrapped(
        font, (const uint16_t*)msg.c_str(),
        SDL_Color{0, 255, 255, 0}, 300
    );

    SDL_Rect rect {
        (int)pos.x, (int)pos.y, text_box->w, text_box->h
    };

    SDL_Texture* texture = SDL_CreateTextureFromSurface(
        renderer, text_box
    );

    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);
}
