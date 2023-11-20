#include "proto.h"

extern SDL_Renderer* renderer;

void print_msg(
    const std::wstring& msg, SDL_Rect bound, TTF_Font* font,
    SDL_Color text_color
) {
    SDL_Surface* text_box = TTF_RenderUNICODE_Blended_Wrapped(
        font, (const uint16_t*)msg.c_str(),
        text_color, bound.w
    );

    SDL_Rect rect {
        bound.x, bound.y, text_box->w, text_box->h
    };

    SDL_Texture* texture = SDL_CreateTextureFromSurface(
        renderer, text_box
    );

    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);
}
