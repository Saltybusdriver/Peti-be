#ifndef MISSILE_H
#define MISSILE_H

#include <SDL.h>
class Missile {
public:
    Missile(float startX, float startY, float velocityX, float velocityY);

    void update(float deltaTime,float backgroundX);
    void render(SDL_Renderer* renderer, SDL_Texture* missileTexture);

private:
    float posX;
    float posY;
    float velX;
    float velY;
};

#endif // MISSILE_H
