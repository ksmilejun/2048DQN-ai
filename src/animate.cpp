#include "game.hpp"
#include <iostream>

#define ANIM_SHIFT_TIME 100
#define ANIM_MERGE_TIME 100
#define ANIM_SPAWN_TIME 50

void Game::animBeginWithX() {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (tiles_[x][y]) {
                anim_start_pos_[x][y] = tiles_[x][y]->pos.x;
            }
        }
    }

    anim_started_ = false;
}

void Game::animBeginWithY() {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (tiles_[x][y]) {
                anim_start_pos_[x][y] = tiles_[x][y]->pos.y;
            }
        }
    }

    anim_started_ = false;
}

void Game::animateShift() {
    auto now = SDL_GetTicks();

    if (!anim_started_) {
        anim_start_time_ = now;
        anim_started_ = true;
    } else if (now - anim_start_time_ > ANIM_SHIFT_TIME) {
        snapTilesToSites();
        state_ = GameState::ANIM_MERGE;
        anim_started_ = false;
        return;
    }

    float t = (float)(now - anim_start_time_) / ANIM_SHIFT_TIME;

    // Smooth Step
    float S = t * t * (3.0f - 2.0f * t);

    if (x_axis_) {
        smoothShiftHorizontal(S);
    } else {
        smoothShiftVertical(S);
    }
}

void Game::smoothShiftHorizontal(float S) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (tiles_[x][y]) {
                float x_start = anim_start_pos_[x][y];
                float x_diff = board_sites_[x][y].x - x_start;
                tiles_[x][y]->pos.x = x_diff * S + x_start;

                if (tiles_[x][y]->merging_with >= 0) {
                    auto& mtile = merged_tiles_[tiles_[x][y]->merging_with];

                    x_start = mtile.start_pos;
                    x_diff = board_sites_[x][y].x - x_start;
                    mtile.tile->pos.x = x_diff * S + x_start;
                }
            }
        }
    }
}

void Game::smoothShiftVertical(float S) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (tiles_[x][y]) {
                float y_start = anim_start_pos_[x][y];
                float y_diff = board_sites_[x][y].y - y_start;
                tiles_[x][y]->pos.y = y_diff * S + y_start;

                if (tiles_[x][y]->merging_with >= 0) {
                    auto& mtile = merged_tiles_[tiles_[x][y]->merging_with];

                    y_start = mtile.start_pos;
                    y_diff = board_sites_[x][y].y - y_start;
                    mtile.tile->pos.y = y_diff * S + y_start;
                }
            }
        }
    }
}

/* -------------------------------------------------------------------------- */

void Game::animateMerge() {
    auto now = SDL_GetTicks();
    
    
    if (!anim_started_) {
        anim_started_ = true;
        anim_start_time_ = now;

        merged_tiles_.clear();

        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                if (tiles_[x][y] && tiles_[x][y]->grow) {
                    tiles_[x][y]->value *= 2;
                    std::cout << tiles_[x][y]->value;
                    tiles_[x][y]->merging_with = -1;
                }
            }
        }
    } else if (now - anim_start_time_ > ANIM_MERGE_TIME) {
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                if (tiles_[x][y]) {
                    tiles_[x][y]->grow = false;
                    tiles_[x][y]->scale = 1.0;
                }
            }
        }

        anim_started_ = false;
        state_ = spawn_x_ == -1 ? GameState::PLAYING : GameState::ANIM_SPAWN_TILE;
        return;
    }

    float t = (float)(now - anim_start_time_) / ANIM_MERGE_TIME;
    
    float B = 2 * t * t * (t * t - 2 * t + 1) + 1;

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (tiles_[x][y] && tiles_[x][y]->grow) {
                tiles_[x][y]->scale = B;
            }
        }
    }
}

/* -------------------------------------------------------------------------- */

void Game::animateSpawnTile() {
    auto now = SDL_GetTicks();

    if (!anim_started_) {
        anim_start_time_ = now;
        anim_started_ = true;

        int spawn_value = rand() % 10 == 0 ? 4 : 2;
        tiles_[spawn_x_][spawn_y_] = newTile(spawn_value, spawn_x_, spawn_y_, 0.0f);
    } else if (now - anim_start_time_ > ANIM_SPAWN_TIME) {
        tiles_[spawn_x_][spawn_y_]->scale = 1.0f;
        spawn_x_ = -1;
        spawn_y_ = -1;

        anim_started_ = false;
        state_ = GameState::PLAYING;
        return;
    }

    float t = (float)(now - anim_start_time_) / ANIM_SPAWN_TIME;
    float S = t * t * (3.0f - 2.0f * t);

    tiles_[spawn_x_][spawn_y_]->scale = 0.5f * S + 0.5f;
}
