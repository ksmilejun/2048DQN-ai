#include "game.hpp"

#include <iostream>

void Game::updateLogic() {
    board_updated_ = false;

    if (hasWon()) {
        state_ = GameState::WON;
        return;
    }

    switch (action_) {
    case Direction::LEFT:
        shiftLeft();
        mergeLeft();
        shiftLeft();

        x_axis_ = true;
        break;
    case Direction::RIGHT:
        shiftRight();
        mergeRight();
        shiftRight();

        x_axis_ = true;
        break;
    case Direction::UP:
        shiftUp();
        mergeUp();
        shiftUp();

        x_axis_ = false;
        break;
    case Direction::DOWN:
        shiftDown();
        mergeDown();
        shiftDown();

        x_axis_ = false;
        break;
    }

    action_ = Direction::NONE;

    if (board_updated_) {
        if (x_axis_) {
            animBeginWithX();
        } else {
            animBeginWithY();
        }

        tryFindTileSpawn();

        state_ = GameState::ANIM_SHIFT;
    } else if (hasLost()) {
        state_ = GameState::LOST;
    }
}

void Game::shiftLeft() {
    for (int y = 0; y < 4; y++) {
        for (int x = 1; x < 4; x++) {
            if (tiles_[x][y]) {
                int new_x = x;
                for (int k = x - 1; k >= 0; k--) {
                    if (tiles_[k][y]) {
                        break;
                    } else {
                        new_x = k;
                    }
                }

                if (new_x != x) {
                    tiles_[new_x][y] = std::move(tiles_[x][y]);
                    board_updated_ = true;
                }
            } 
        }
    }
}

void Game::shiftRight() {
    for (int y = 0; y < 4; y++) {
        for (int x = 2; x >= 0; x--) {
            if (tiles_[x][y]) {
                int new_x = x;
                for (int k = x + 1; k < 4; k++) {
                    if (tiles_[k][y]) {
                        break;
                    } else {
                        new_x = k;
                    }
                }

                if (new_x != x) {
                    tiles_[new_x][y] = std::move(tiles_[x][y]);
                    board_updated_ = true;
                }
            }
        }
    }
}

void Game::shiftUp() {
    for (int x = 0; x < 4; x++) {
        for (int y = 1; y < 4; y++) {
            if (tiles_[x][y]) {
                int new_y = y;
                for (int k = y - 1; k >= 0; k--) {
                    if (tiles_[x][k]) {
                        break;
                    } else {
                        new_y = k;
                    }
                }

                if (new_y != y) {
                    tiles_[x][new_y] = std::move(tiles_[x][y]);
                    board_updated_ = true;
                }
            }
            
        }
    }
}

void Game::shiftDown() {
    for (int x = 0; x < 4; x++) {
        for (int y = 2; y >= 0; y--) {
            if (tiles_[x][y]) {
                int new_y = y;
                for (int k = y + 1; k < 4; k++) {
                    if (tiles_[x][k]) {
                        break;
                    } else {
                        new_y = k;
                    }
                }

                if (new_y != y) {
                    tiles_[x][new_y] = std::move(tiles_[x][y]);
                    board_updated_ = true;
                }
            }
        }
    }
}

/* -------------------------------------------------------------------------- */

void Game::mergeLeft() {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 3; x++) {
            if (tiles_[x][y] && tiles_[x+1][y] && tiles_[x][y]->value == tiles_[x+1][y]->value) {
                tiles_[x][y]->grow = true;
                mergeTileX(x+1, y, x);
                board_updated_ = true;
            }
        }
    }
}

void Game::mergeRight() {
    for (int y = 0; y < 4; y++) {
        for (int x = 3; x > 0; x--) {
            if (tiles_[x][y] && tiles_[x-1][y] && tiles_[x][y]->value == tiles_[x-1][y]->value) {
                tiles_[x][y]->grow = true;
                mergeTileX(x-1, y, x);
                board_updated_ = true;
            }
        }
    }
}

void Game::mergeUp() {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 3; y++) {
            if (tiles_[x][y] && tiles_[x][y+1] && tiles_[x][y]->value == tiles_[x][y+1]->value) {
                tiles_[x][y]->grow = true;
                mergeTileY(x, y+1, y);
                board_updated_ = true;
            }
        }
    }
}

void Game::mergeDown() {
    for (int x = 0; x < 4; x++) {
        for (int y = 3; y > 0; y--) {
            if (tiles_[x][y] && tiles_[x][y-1] && tiles_[x][y]->value == tiles_[x][y-1]->value) {
                tiles_[x][y]->grow = true;
                mergeTileY(x, y-1, y);
                board_updated_ = true;
            }
        }
    }
}

/* -------------------------------------------------------------------------- */

void Game::mergeTileX(int ix, int iy, int dest) {
    float start_pos = tiles_[ix][iy]->pos.x;

    tiles_[dest][iy]->merging_with = merged_tiles_.size();

    merged_tiles_.emplace_back(MergedTile{std::move(tiles_[ix][iy]), start_pos});
}

void Game::mergeTileY(int ix, int iy, int dest) {
    float start_pos = tiles_[ix][iy]->pos.y;

    tiles_[ix][dest]->merging_with = merged_tiles_.size();

    merged_tiles_.emplace_back(MergedTile{std::move(tiles_[ix][iy]), start_pos});
}

/* -------------------------------------------------------------------------- */

struct IVec2 {
    int x, y;
};

void Game::tryFindTileSpawn() {
    std::vector<IVec2> empty_spots;
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (!tiles_[x][y])
                empty_spots.push_back({x, y});
        }
    }

    if (empty_spots.size() == 0) {
        spawn_x_ = -1;
        spawn_y_ = -1;
        return;
    }

    auto& spawn_pos = empty_spots[rand() % empty_spots.size()];
    spawn_x_ = spawn_pos.x;
    spawn_y_ = spawn_pos.y;
}

/* -------------------------------------------------------------------------- */

bool Game::hasWon() {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (tiles_[x][y] && tiles_[x][y]->value == 2048) {
                return true;
            }
        }
    }

    return false;
}

bool Game::hasLost() {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (!tiles_[x][y])
                return false;

            if (x < 3 && (!tiles_[x+1][y] || tiles_[x][y]->value == tiles_[x+1][y]->value))
                return false;

            if (y < 3 && (!tiles_[x][y+1] || tiles_[x][y]->value == tiles_[x][y+1]->value))
                return false;  
        }
    }

    return true;
}

void Game::resetGame() {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            tiles_[x][y].reset();
        }
    }

    tryFindTileSpawn();

    state_ = GameState::ANIM_SPAWN_TILE;
}