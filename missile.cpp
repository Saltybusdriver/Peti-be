#include "missile.h"
#include <iostream>
#include <SDL.h>
Missile::Missile(float startX, float startY, float velocityX, float velocityY)
    : posX(startX), posY(startY), velX(velocityX), velY(velocityY) {}


void Missile::render(SDL_Renderer* renderer, SDL_Texture* missileTexture)
{
    SDL_Rect missileRect = { static_cast<int>(posX), static_cast<int>(posY), 300, 300 };
    if (missileTexture == nullptr) {
        std::cerr << "Failed to render missile: Texture is null\n";
    } 
    else {
        SDL_RenderCopy(renderer, missileTexture, NULL, &missileRect);
    }
}
void Missile::update(float deltaTime,float backgroundX) {
    posX += velX * deltaTime-backgroundX;
    posY += velY * deltaTime-backgroundX;
}
