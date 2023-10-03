#include "renderer.hpp"

#include <iostream>

#define FONT_PATH "assets/OpenSans-Regular.ttf"

void reportSDLError(const std::string& message) {
    std::cout << "error: " << message << ": " << SDL_GetError() << '\n';
}

/* -------------------------------------------------------------------------- */

bool Renderer::Init() {
    window_ = SDL_CreateWindow("2048", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window_) {
        reportSDLError("creating window");
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_) {
        reportSDLError("creating renderer");
        return false;
    }
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);

    font_ = TTF_OpenFont(FONT_PATH, 40);
    if (!font_) {
        reportSDLError("opening font");
        return false;
    }

    return true;
}

void Renderer::DrawRect(float x, float y, float w, float h, SDL_Color color) {
    SDL_Rect rect{(int)x, (int)y, (int)w, (int)h};

    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer_, &rect);
}

void Renderer::DrawText(const std::string& text, float x, float y, SDL_Color color) {
    auto* surf = TTF_RenderText_Blended(font_, text.c_str(), color);
    if (!surf) {
        reportSDLError("rendering text");
        return;
    }

    auto* tx = SDL_CreateTextureFromSurface(renderer_, surf);
    SDL_FreeSurface(surf);
    if (!tx) {
        reportSDLError("creating texture from text");
        return;
    }

    int txWidth, txHeight;
    SDL_QueryTexture(tx, nullptr, nullptr, &txWidth, &txHeight);

    SDL_Rect dest{
        (int)x - txWidth / 2, (int)y - txHeight / 2,
        txWidth, txHeight
    };

    SDL_RenderCopy(renderer_, tx, nullptr, &dest);
}

Renderer::~Renderer() {
    if (renderer_)
        SDL_DestroyRenderer(renderer_);

    if (window_)
        SDL_DestroyWindow(window_);

    SDL_Quit();
}
