#include "game.h"
#include <iostream>

Game::Game(){
    gWindow = nullptr;
    gRenderer = nullptr;
    backgroundTexture = nullptr;
    backgroundX=0.0f;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';
        exit(1);
    }
    if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
        std::cerr << "SDL_GetCurrentDisplayMode failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }
    SCREEN_WIDTH=dm.w;
    SCREEN_HEIGHT=dm.h;
    BACKGROUND_WIDTH=SCREEN_WIDTH*2;
    BACKGROUND_HEIGHT=SCREEN_HEIGHT;
    
    helicopterInstance = Helicopter(&SCREEN_WIDTH, &SCREEN_HEIGHT);
}

Game::~Game() {
    close();
}

bool Game::init() {
    
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << '\n';
        return false;
    }
    
    gWindow = SDL_CreateWindow("Helicopter Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, dm.w, dm.h, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << '\n';
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << '\n';
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << '\n';
        return false;
    }

    heliTexture = IMG_LoadTexture(gRenderer, "Assets/heli.png");
    if (heliTexture == nullptr) {
        std::cerr << "Failed to load helicopter image! SDL_image Error: " << IMG_GetError() << '\n';
        return false;
    }
    missileTexture = IMG_LoadTexture(gRenderer, "Assets/missile.png");
    if (missileTexture == nullptr) {
        std::cerr << "Failed to load helicopter image! SDL_image Error: " << IMG_GetError() << '\n';
        return false;
    }
    backgroundTexture = IMG_LoadTexture(gRenderer, "Assets/background.png");
    if (backgroundTexture == nullptr) {
        std::cerr << "Failed to load background image! SDL_image Error: " << IMG_GetError() << '\n';
        return false;
    }

    return true;
}

void Game::close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;
    SDL_DestroyTexture(backgroundTexture);
    backgroundTexture = nullptr;
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void Game::spawnMissiles() {
    // Spawn missiles off-screen with a specific direction and velocity
    // Example: spawn missile from the right side of the screen
    float startX = SCREEN_WIDTH;
    float startY = rand() % SCREEN_HEIGHT; // Random Y position
    float velocityX = -50.0f; // Negative X velocity to move left
    float velocityY = 0.0f; // No vertical velocity
    missiles.push_back(Missile(startX, startY, velocityX, velocityY));
}

void Game::updateMissiles(float deltaTime,float backgroundX) {
    // Update position of each active missile
    for (auto& missile : missiles) {
        missile.update(deltaTime,backgroundX);

        // Check if missile is off-screen and handle deletion/recycling
        // Example: if (missile.isOffScreen()) { /* Handle deletion/recycling */ }
    }
}


void Game::run() {
    bool quit = false;
    Uint32 lastFrameTime = SDL_GetTicks(); // Time of the last frame
    
    while (!quit) {
        Uint32 currentFrameTime = SDL_GetTicks(); // Current time
        float deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f; // Delta time in seconds
        lastFrameTime = currentFrameTime; // Update last frame time
        
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            helicopterInstance.handleEvents(event);
        }
        backgroundX -= helicopterInstance.getMoveX();
        spawnMissiles();
        updateMissiles(deltaTime,backgroundX);
        if (backgroundX < -BACKGROUND_WIDTH) 
        {
            backgroundX += BACKGROUND_WIDTH; 
        }
        else if (backgroundX > 0)
        {
            backgroundX -= BACKGROUND_WIDTH;
        }
        helicopterInstance.update(deltaTime);
        
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        
        SDL_Rect bgRect1 = { static_cast<int>(backgroundX), 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT };
        SDL_Rect bgRect2 = { static_cast<int>(backgroundX) + BACKGROUND_WIDTH, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT };
        SDL_RenderCopy(gRenderer, backgroundTexture, NULL, &bgRect1);
        SDL_RenderCopy(gRenderer, backgroundTexture, NULL, &bgRect2);
        for (auto& missile : missiles) 
        {
            missile.render(gRenderer, missileTexture);
        }   
        helicopterInstance.render(gRenderer,heliTexture);

        SDL_RenderPresent(gRenderer);
    }
}
