#include "game.hpp"
#include <iostream>
#include "time.h"

Game::Game(Renderer& renderer)
: renderer_(renderer)
{
    calcBoardSites();

    tryFindTileSpawn();
    state_ = GameState::ANIM_SPAWN_TILE;
}

void Game::Run() {
    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return;
        }

        double input[16];
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                input[i * 4 + j] = tiles_[i][j]->value; // starts at left top, ends at right bottom
            }
        }

        ilayer.processlayer(input);
        hlayer1.processlayer(ilayer.outputdata);
        hlayer2.processlayer(hlayer1.outputdata);
        hlayer3.processlayer(hlayer2.outputdata);
        olayer.processlayer(hlayer3.outputdata);

        int index = 0;

        for (int i = 0; i < 4; i++) {
            int max = 0;
            if (max < olayer.outputdata[i]) {
                max = olayer.outputdata[i]; index = i;
            }
        }

        switch (index) {
        case 0:
            action_ = Direction::UP;
        case 1:
            action_ = Direction::DOWN;
        case 2:
            action_ = Direction::LEFT;
        case 3:
            action_ = Direction::RIGHT;
        }

        renderer_.Clear();

        handleInput();

        switch (state_) {
        case GameState::PLAYING:
            updateLogic();
            break;
        case GameState::ANIM_SHIFT:
            animateShift();
            break;
        case GameState::ANIM_MERGE:
            animateMerge();
            break;
        case GameState::ANIM_SPAWN_TILE:
            animateSpawnTile();
            break;
        case GameState::WON:
        case GameState::LOST:
            if (hitSpace()) {
                resetGame();
            }
            break;
        }

        draw();

        renderer_.Present();
        
    }
}

/* -------------------------------------------------------------------------- */

int main() {
    srand(time(nullptr));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        reportSDLError("initializing SDL");
        return -1;
    }

    if (TTF_Init() < 0) {
        reportSDLError("initializing TTF library");
        SDL_Quit();
        return -1;
    }

    Renderer renderer;
    if (!renderer.Init()) {
        return -1;
    }

    Game game(renderer);

    for (int i = 0; i < 10; i++) {
        game.search();
        game.ai_learn();
    }

    game.Run();

    return 0;
}