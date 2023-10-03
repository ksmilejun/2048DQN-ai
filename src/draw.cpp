#include "game.hpp"

#include <unordered_map>

#define BOARD_SIZE WINDOW_WIDTH
#define TILE_PADDING 20
#define TILE_SIZE (((BOARD_SIZE - 5 * TILE_PADDING) / 4))
#define BOARD_OFFSET ((WINDOW_HEIGHT - BOARD_SIZE))

/* -------------------------------------------------------------------------- */

const SDL_Color CLR_BOARD_BG{187, 173, 160, 255};
const SDL_Color CLR_FONT_LIGHT{249, 246, 242, 255};
const SDL_Color CLR_FONT_DARK{119, 110, 101, 255};
const SDL_Color CLR_SUPER_TILE{60, 58, 50, 255};
const SDL_Color CLR_TILE_EMPTY{238, 228, 218, 89};
const SDL_Color CLR_END_OVERLAY{237, 194, 46, 125};

std::unordered_map<int, SDL_Color> value_to_color {
    { 2, {238, 228, 217, 255} },
    { 4, {237, 224, 200, 255} },
    { 8, {242, 177, 121, 255} },
    { 16, {245, 149, 99, 255} },
    { 32, {246, 128, 95, 255} },
    { 64, {246, 94, 59, 255} },
    { 128, {237, 207, 114, 255} },
    { 256, {237, 204, 97, 255} },
    { 512, {237, 200, 80, 255} },
    { 1024, {237, 197, 63, 255} },
    { 2048, {237, 194, 46, 255} }
};

/* -------------------------------------------------------------------------- */

void Game::calcBoardSites() {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            float site_x = (TILE_SIZE + TILE_PADDING) * x + TILE_PADDING;
            float site_y = (TILE_SIZE + TILE_PADDING) * y + TILE_PADDING + BOARD_OFFSET;

            board_sites_[x][y] = {site_x, site_y};
        }
    }
}

void Game::snapTilesToSites() {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (tiles_[x][y])
                tiles_[x][y]->pos = board_sites_[x][y];
        }
    }
}

void Game::draw() {
    renderer_.DrawRect(0, BOARD_OFFSET, BOARD_SIZE, BOARD_SIZE, CLR_BOARD_BG);

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            Vec2 pos = board_sites_[x][y];
            renderer_.DrawRect(pos.x, pos.y, TILE_SIZE, TILE_SIZE, CLR_TILE_EMPTY);
        }
    }

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (tiles_[x][y]) {
                drawTile(tiles_[x][y]);
            }
        }
    }

    for (auto& mtile : merged_tiles_) {
        drawTile(mtile.tile);
    }

    if (state_ == GameState::WON || state_ == GameState::LOST) {
        renderer_.DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, CLR_END_OVERLAY);

        renderer_.DrawText(
            state_ == GameState::WON ? "You win!" : "You lose.",
            WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50, CLR_FONT_LIGHT
        );
        renderer_.DrawText("Press Space to Play Again", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 50, CLR_FONT_LIGHT);
    }
}

void Game::drawTile(const std::unique_ptr<Tile>& tile) {
    float size = TILE_SIZE * tile->scale;
    float scale_offset = (size - TILE_SIZE) / 2;

    auto& pair = value_to_color.find(tile->value);
    if (pair == value_to_color.end()) {
        renderer_.DrawRect(tile->pos.x - scale_offset, tile->pos.y - scale_offset, size, size, CLR_SUPER_TILE);
    } else {
        renderer_.DrawRect(tile->pos.x - scale_offset, tile->pos.y - scale_offset, size, size, pair->second);
    }

    renderer_.DrawText(std::to_string(tile->value), 
        tile->pos.x + TILE_SIZE / 2, tile->pos.y + TILE_SIZE / 2, 
        tile->value < 8 ? CLR_FONT_DARK : CLR_FONT_LIGHT
    );
}