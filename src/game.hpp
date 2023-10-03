#ifndef GAME_H_INC
#define GAME_H_INC

#include <memory>
#include <vector>

#include "renderer.hpp"
#include "neuron.hpp"
#include "utils.hpp"

layer ilayer(16);
layer hlayer1(16);
layer hlayer2(16);
layer hlayer3(16);
layer olayer(4); //0:up 1:down 2:left 3:right

struct Vec2 {
    float x, y;
};

enum class direction
{
    NORTH = 0,
    SOUTH,
    EAST,
    WEST
};

struct Tile {
    int value { 0 };
    Vec2 pos;
    float scale { 1.0f };
    bool grow { false };
    int merging_with { -1 };

    Tile(int value, Vec2 pos, float scale = 1.0)
    : value(value), pos(pos), scale(scale)
    {} 
};

struct MergedTile {
    std::unique_ptr<Tile> tile;
    float start_pos;
};

enum class GameState {
    PLAYING,
    ANIM_SHIFT,
    ANIM_MERGE,
    ANIM_SPAWN_TILE,
    WON,
    LOST
};

enum class Direction {
    NONE,
    LEFT,
    RIGHT,
    UP, 
    DOWN
};

class Game {
    Renderer& renderer_;
    GameState state_ { GameState::PLAYING };
    std::unique_ptr<Tile> tiles_[4][4];
    std::vector<MergedTile> merged_tiles_;

    // Input State
    Direction input_dir_ { Direction::NONE };
    Direction action_ { Direction::NONE };

    // Logic State
    bool board_updated_ { false };
    int spawn_x_ {-1}, spawn_y_ {-1};
    bool x_axis_;

    // Drawing State
    Vec2 board_sites_[4][4];

    // Animation State
    float anim_start_pos_[4][4];
    bool anim_started_ { false };
    Uint32 anim_start_time_;

    std::vector <direction> result;
    struct gridtemp { int gridmap[4][4]; };
    std::vector<std::vector <gridtemp>>m_grid_rec;
    std::vector<gridtemp>m_grid_temp;
    std::vector<gridtemp>m_grid_result;
    std::vector <std::vector<direction>> d;

public:
    Game(Renderer& renderer);

    void Run();

    void ai_learn();

    void search();

    int m_grid[4][4];

private:
    void handleInput();
    bool hitSpace();

    /* ---------------------------------------------------------------------- */

    void updateLogic();
    
    void shiftLeft();
    void shiftRight();
    void shiftUp();
    void shiftDown();

    void mergeLeft();
    void mergeRight();
    void mergeUp();
    void mergeDown();

    void mergeTileX(int ix, int iy, int dest);
    void mergeTileY(int ix, int iy, int dest);

    void tryFindTileSpawn();

    bool hasWon();
    bool hasLost();
    void resetGame();

    /* ---------------------------------------------------------------------- */

    void calcBoardSites();
    void snapTilesToSites();
    void draw();
    void drawTile(const std::unique_ptr<Tile>& tile);

    /* ---------------------------------------------------------------------- */

    void animBeginWithX();
    void animBeginWithY();
    void animateShift();

    void smoothShiftHorizontal(float S);
    void smoothShiftVertical(float S);

    void animateMerge();

    void animateSpawnTile();
    void ai();


    /* ---------------------------------------------------------------------- */

    inline std::unique_ptr<Tile> newTile(int value, int ix, int iy, float scale = 1.0f) {
        return std::make_unique<Tile>(value, board_sites_[ix][iy], scale);
    }
};

#endif