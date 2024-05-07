#include "helicopter.h"
#include <iostream>
#include <cmath>
Helicopter::Helicopter(){}
Helicopter::Helicopter(int* _SCREEN_WIDTH,int* _SCREEN_HEIGHT) {
    heliX = *_SCREEN_WIDTH / 2.0f;
    heliY = *_SCREEN_HEIGHT / 2.0f;
    heliYVelocity = 0.0f;
    tilt = 0.0f;
    EnginePower = 9.8f;
    EngineOffset = 32.0f;
    HELI_SPEED = 0.0f;
    moveX = 0.0f;
    moveY = 0.0f;
    SCREEN_WIDTH=_SCREEN_WIDTH;
    SCREEN_HEIGHT=_SCREEN_HEIGHT;
}

Helicopter::~Helicopter() {
    // Destructor
}

float Helicopter::getMoveX()
{
    return moveX;
}
float Helicopter::getMoveY()
{
    return moveY;
}
void Helicopter::handleEvents(SDL_Event& event) {
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            SDL_Quit();
            exit(0);
        }
    }
}

void Helicopter::update(float deltaTime) {
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    if (currentKeyStates[SDL_SCANCODE_UP]) {
        EnginePower += EngineOffset * deltaTime;
    }
    if (currentKeyStates[SDL_SCANCODE_DOWN]) {
        EnginePower -= EngineOffset * deltaTime;
    }
    if (currentKeyStates[SDL_SCANCODE_LEFT]) {
        tilt -= TILT_SENSITIVITY * deltaTime; // Tilt left
    }
    if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
        tilt += TILT_SENSITIVITY * deltaTime; // Tilt right
    }

    heliYVelocity = (GRAVITY - EnginePower);
    HELI_SPEED = std::min(MAX_HORIZONTAL_SPEED, std::abs(heliYVelocity) * tilt);

    float movementDistance = HELI_SPEED * deltaTime;

    float overallSpeed = HELI_SPEED + heliYVelocity - GRAVITY * deltaTime;

    moveX = HELI_SPEED * deltaTime;
    moveY = heliYVelocity * deltaTime;

    heliX += moveX;
    heliY += moveY;

    heliX = std::max(0.0f, std::min(heliX, static_cast<float>(*SCREEN_WIDTH - HELI_WIDTH)));
    heliY = std::max(0.0f, std::min(heliY, static_cast<float>(*SCREEN_HEIGHT - HELI_HEIGHT)));
}

void Helicopter::render(SDL_Renderer* renderer,SDL_Texture* heliTexture) {
    SDL_Rect heliRect = { static_cast<int>(heliX), static_cast<int>(heliY), HELI_WIDTH, HELI_HEIGHT };
    float tiltAngle = std::atan2(tilt, HELI_SPEED) * 180.0f/ M_PI;
    if (heliTexture == nullptr) {
        std::cerr << "Failed to render helicopter image: Texture is null\n";
    } 
    else 
    {
        SDL_RenderCopyEx(renderer, heliTexture, NULL, &heliRect, tilt, NULL, SDL_FLIP_NONE);
    }
}
