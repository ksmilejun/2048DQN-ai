#include "game.hpp"
#include <iostream>

void Game::handleInput() {
    auto* ks = SDL_GetKeyboardState(nullptr);

    if (ks[SDL_SCANCODE_LEFT]) {
        if (input_dir_ != Direction::LEFT) {
            input_dir_ = Direction::LEFT;
            action_ = input_dir_;
            return;
        }
    } else if (ks[SDL_SCANCODE_RIGHT]) {
        if (input_dir_ != Direction::RIGHT) {
            input_dir_ = Direction::RIGHT;
            action_ = input_dir_;
            return;
        }
    } else if (ks[SDL_SCANCODE_UP]) {
        if (input_dir_ != Direction::UP) {
            input_dir_ = Direction::UP;
            action_ = input_dir_;
            return;
        }
    } else if (ks[SDL_SCANCODE_DOWN]) {
        if (input_dir_ != Direction::DOWN) {
            input_dir_ = Direction::DOWN;
            action_ = input_dir_;
            return;
        }
    } else {
        input_dir_ = Direction::NONE;
    }
    
}

bool Game::hitSpace() {
    auto* ks = SDL_GetKeyboardState(nullptr);
    return ks[SDL_SCANCODE_SPACE];
}