#ifndef RENDERER_H_INC
#define RENDERER_H_INC

#include <string>

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_ttf.h"

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

void reportSDLError(const std::string& message);

class Renderer {
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    TTF_Font* font_;

public:
    bool Init();
    
    inline void Clear() {
        SDL_SetRenderDrawColor(renderer_, 250, 248, 239, 255);
        SDL_RenderClear(renderer_);
    }

    inline void Present() {
        SDL_RenderPresent(renderer_);
    }

    void DrawRect(float x, float y, float w, float h, SDL_Color color);
    void DrawText(const std::string& text, float x, float y, SDL_Color color);

    ~Renderer();
};

#endif