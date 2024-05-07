#include <SDL.h>
#include <iostream>
#include <cmath> 
#include <SDL_image.h>
#include <SDL_ttf.h>
// Screen dimensions
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int BACKGROUND_WIDTH = 2048; // Twice the screen width for seamless looping
const int BACKGROUND_HEIGHT = 768;
float backgroundX = 0.0f; // Add this line
// Helicopter dimensions
const int HELI_WIDTH = 200;
const int HELI_HEIGHT = 200;
const float MAX_HORIZONTAL_SPEED = 300.0f; // Pixels per second

const float GRAVITY = 9.8f; // Gravity acceleration (m/s^2)
const float TILT_SENSITIVITY = 8.0f; // Sensitivity for tilt adjustment
float HELI_SPEED=0.0f;
float tilt =0.0f;
float heliYVelocity = 0.0f; 
float EnginePower=9.8f;
float EngineOffset=32.0f;
// SDL variables
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* heliTexture = nullptr;
SDL_Texture* backgroundTexture = nullptr;
SDL_Event event;

// Helicopter variables
float heliX = SCREEN_WIDTH / 2.0f;
float heliY = SCREEN_HEIGHT / 2.0f;

// Function prototypes
bool init();
void close();
void handleEvents();
void update(float deltaTime); // Pass deltaTime
void render();

int main(int argc, char* args[]) {
    if (!init()) {
        std::cerr << "Failed to initialize SDL.\n";
        return -1;
    }

    bool quit = false;
    Uint32 lastFrameTime = SDL_GetTicks(); // Time of the last frame

    while (!quit) {
        Uint32 currentFrameTime = SDL_GetTicks(); // Current time
        float deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f; // Delta time in seconds
        lastFrameTime = currentFrameTime; // Update last frame time

        handleEvents();
        update(deltaTime); // Pass deltaTime
        render();
    }

    close();
    return 0;
}

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';
        return false;
    }
    if (TTF_Init() == -1) {
        // Handle initialization error
        return 1;
    }
    gWindow = SDL_CreateWindow("Helicopter Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

    // Load helicopter texture
    heliTexture = IMG_LoadTexture(gRenderer, "Assets/heli.png");
    if (heliTexture == nullptr) {
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

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;
    SDL_DestroyTexture(heliTexture);
    heliTexture = nullptr;
    SDL_DestroyTexture(backgroundTexture);
    backgroundTexture = nullptr;
    IMG_Quit();
    SDL_Quit();
}

void handleEvents() {
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            SDL_Quit();
            exit(0);
        }
    }
}

void update(float deltaTime) {
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
   
    

    if (currentKeyStates[SDL_SCANCODE_UP]) {
        EnginePower+=EngineOffset * deltaTime; 
    }
    if (currentKeyStates[SDL_SCANCODE_DOWN]) {
        EnginePower-=EngineOffset * deltaTime;
    }
    if (currentKeyStates[SDL_SCANCODE_LEFT]) {
        tilt -= TILT_SENSITIVITY * deltaTime; // Tilt left
    }
    if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
        tilt += TILT_SENSITIVITY * deltaTime; // Tilt right
    }

    heliYVelocity = (GRAVITY-EnginePower);
    //tilt = std::atan2(-heliYVelocity, HELI_SPEED) * TILT_SENSITIVITY;
  
    HELI_SPEED = std::min(MAX_HORIZONTAL_SPEED, std::abs(heliYVelocity)*tilt);

    float movementDistance = HELI_SPEED * deltaTime;

    float overallSpeed = HELI_SPEED + heliYVelocity - GRAVITY*deltaTime;

  

    float moveX = HELI_SPEED * deltaTime;
    float moveY = heliYVelocity * deltaTime;
 
    heliX += moveX;
    heliY += moveY;


    heliX = std::max(0.0f, std::min(heliX, static_cast<float>(SCREEN_WIDTH - HELI_WIDTH)));
    heliY = std::max(0.0f, std::min(heliY, static_cast<float>(SCREEN_HEIGHT - HELI_HEIGHT)));

    backgroundX -= moveX;
    if (backgroundX < -BACKGROUND_WIDTH) {
        backgroundX += BACKGROUND_WIDTH; // Move background back to the right side
    }
    else if (backgroundX > 0) {
        backgroundX -= BACKGROUND_WIDTH; // Move background back to the right side
    }
}

void render() {
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    SDL_Rect heliRect = { static_cast<int>(heliX), static_cast<int>(heliY), HELI_WIDTH, HELI_HEIGHT };
    float tiltAngle = std::atan2(tilt, HELI_SPEED) * 180.0f/ M_PI;
    SDL_Rect bgRect1 = { static_cast<int>(backgroundX), 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT };
    SDL_Rect bgRect2 = { static_cast<int>(backgroundX) + BACKGROUND_WIDTH, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT };
    SDL_RenderCopy(gRenderer, backgroundTexture, NULL, &bgRect1);
    SDL_RenderCopy(gRenderer, backgroundTexture, NULL, &bgRect2);
    if (heliTexture == nullptr) {
        std::cerr << "Failed to render helicopter image: Texture is null\n";
    } else {
        SDL_RenderCopyEx(gRenderer, heliTexture, NULL, &heliRect, tilt, NULL, SDL_FLIP_NONE);
    }
    // Render variables as text
    SDL_Color textColor = {255, 255, 255}; // White color

    // Render HELI_SPEED
    std::string speedText = "HELI_SPEED: " + std::to_string(HELI_SPEED);
    SDL_Surface* speedSurface = SDL_CreateRGBSurface(0, 200, 40, 32, 0, 0, 0, 0); // Create a surface for text
    if (!speedSurface) {
        std::cerr << "Failed to create surface: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_FillRect(speedSurface, NULL, SDL_MapRGB(speedSurface->format, 0, 0, 0)); // Fill surface with black color
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/noto/NotoSans-Light.ttf", 24); // Use a built-in font (Arial)
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, speedText.c_str(), textColor);
    if (!textSurface) {
        std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }
    SDL_Rect textRect = {10, 10, 0, 0}; // Position of the text on the screen
    SDL_BlitSurface(textSurface, NULL, speedSurface, &textRect); // Blit text surface onto the surface
    SDL_Texture* speedTexture = SDL_CreateTextureFromSurface(gRenderer, speedSurface); // Create texture from the surface
    SDL_FreeSurface(textSurface); // Free the text surface
    SDL_FreeSurface(speedSurface); // Free the surface
    SDL_RenderCopy(gRenderer, speedTexture, NULL, &textRect); // Render the texture
    SDL_DestroyTexture(speedTexture); // Destroy the texture
    TTF_CloseFont(font); // Close the font
    SDL_RenderPresent(gRenderer);
}
