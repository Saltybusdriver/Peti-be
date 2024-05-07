#ifndef HELICOPTER_H
#define HELICOPTER_H

#include <SDL.h>

class Helicopter {
public:
    Helicopter(int* _SCREEN_WIDTH, int* _SCREEN_HEIGHT);
    Helicopter();
    ~Helicopter();
    void handleEvents(SDL_Event& event);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer, SDL_Texture* heliTexture);
    float getMoveX();
    float getMoveY();

private:
    float heliX;
    float heliY;
    float heliYVelocity;
    float tilt;
    float EnginePower;
    float EngineOffset;
    float HELI_SPEED;
    int HELI_WIDTH = 200;
    int HELI_HEIGHT = 200;
    float MAX_HORIZONTAL_SPEED = 300.0f;
    float GRAVITY = 9.8f;
    float TILT_SENSITIVITY = 8.0f;
    int* SCREEN_WIDTH;
    int* SCREEN_HEIGHT;
    float moveX;
    float moveY; 
};

#endif
