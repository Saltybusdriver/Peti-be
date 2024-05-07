#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "helicopter.h"
#include "missile.h"
#include <vector>
class Game {
public:
    Game();
    ~Game();
    bool init();
    void close();
    void run();

    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    float backgroundX;
    int BACKGROUND_WIDTH;
    int BACKGROUND_HEIGHT;
private:

    void spawnMissiles();
    void updateMissiles(float deltaTime,float backgroundX);
    void renderMissiles();

    std::vector<Missile> missiles;
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    Helicopter helicopterInstance;
    SDL_Texture* backgroundTexture;
    SDL_Texture* heliTexture;
    SDL_Texture* missileTexture;
    SDL_Event event;
    SDL_DisplayMode dm;
};

#endif
