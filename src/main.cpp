#include <iostream>
#include <string>
#include <iso646.h>

#include <SDL.h>
#include <SDL_image.h>

// Without this we have an error LNK2019: external unresolved symbol main
#undef main

// Set screen size
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Set tile size
const int TILE_SIZE = 40;

struct Position {
    int x;
    int y;
};

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

// Function defenition
SDL_Texture* loadImage(std::string&& path);
void applySurface(int x, int y, SDL_Texture* texture);
bool renderTexture(SDL_Texture* texture, int x, int y, int w, int h);
bool renderTexture(SDL_Texture* texture, int x, int y);
void cleanup();

void renderBackground(SDL_Texture* background);
bool checkCollision(const Position& pos, int size);

int main(int argc, char** argv) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    // Initialize SDL_Image module
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create window
    window = SDL_CreateWindow("SDL_Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Create Window Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "Create Renderer Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Load texture
    SDL_Texture* background = nullptr;
    SDL_Texture* character = nullptr;

    background = loadImage("background.png");
    character = loadImage("character.png");
    if (!background || !character) {
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(character);
        cleanup();
        return 1;
    }

    bool quit = false;
    SDL_Event event;

    // Chatacter pos
    Position charPos { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    int charSize = 40;

    // main loop
    while (!quit) {
        // Event processing
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN) {
                const uint8_t* state = SDL_GetKeyboardState(NULL);
                // Diagional movement
                if (state[SDL_SCANCODE_W] and state[SDL_SCANCODE_A]) {
                    Position nextPos {charPos.x, charPos.y - charSize};
                    if (!checkCollision(nextPos, charSize))
                        charPos.y -= charSize;

                    nextPos.x = charPos.x - charSize;
                    if (!checkCollision(nextPos, charSize))
                        charPos.x -= charSize;
                }
                else if (state[SDL_SCANCODE_W]) {
                    Position nextPos {charPos.x, charPos.y - charSize};
                    if (!checkCollision(nextPos, charSize))
                        charPos.y -= charSize;
                }
                else if (state[SDL_SCANCODE_S]) {
                    Position nextPos {charPos.x, charPos.y + charSize};
                    if (!checkCollision(nextPos, charSize))
                        charPos.y += charSize;
                }
                else if (state[SDL_SCANCODE_A]) {
                    Position nextPos {charPos.x - charSize, charPos.y};
                    if (!checkCollision(nextPos, charSize))
                        charPos.x -= charSize;
                }
                else if (state[SDL_SCANCODE_D]) {
                    Position nextPos {charPos.x + charSize, charPos.y};
                    if (!checkCollision(nextPos, charSize))
                        charPos.x += charSize;
                }
                else if (state[SDL_SCANCODE_ESCAPE])
                    quit = true;
            }
        }

        // Clear window
        SDL_RenderClear(renderer);
        renderBackground(background);
        renderTexture(character, charPos.x, charPos.y, charSize, charSize);
        SDL_RenderPresent(renderer);

        // Free CPU resources
        SDL_Delay(1);
    }

    cleanup();

    return 0;
}

/**
 * Load texture from file on the path "path"
 * @param  path    Path to the file
 * @return Returns texture or nullptr if error occurred
*/
SDL_Texture* loadImage(std::string&& path) {
    // Load image
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if (!texture) {
        std::cerr << "Load Image Error: " << IMG_GetError() << std::endl;
    }
    return texture;
}

/**
 * Draw image on renderer with scaling
 * @param texture Texture for drawing
 * @param x X coordinate
 * @param y Y coordinate
 * @param w Scaling width
 * @param h Scaling height
 * @return Returns true if success or false
 */
bool renderTexture(SDL_Texture* texture, int x, int y, int w, int h) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    if (SDL_RenderCopy(renderer, texture, NULL, &rect) != 0) {
        std::cerr << "Render Texture Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

/**
 * Draw image on renderer without scaling
 * @param texture Texture for drawing
 * @param x X coordinate
 * @param y Y coordinate
 * @return Returns true if success or false
 */
bool renderTexture(SDL_Texture* texture, int x, int y) { 
    int w, h;
    // Get image size
    if (SDL_QueryTexture(texture, NULL, NULL, &w, &h) != 0) {
        std::cerr << SDL_GetError() << std::endl;
        return false;
    }

    if (renderTexture(texture, x, y, w, h) == false) {
        return false;
    }
    return true;
}

/**
 * Free resources
 * @return None
 */
void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

/**
 * Draw background
 * @param background Pointer to image
 * @return None
 */
void renderBackground(SDL_Texture* background) {
    int xTiles = SCREEN_WIDTH / TILE_SIZE;
    int yTiles = SCREEN_HEIGHT / TILE_SIZE;

    for (int i = 0; i < xTiles * yTiles; ++i) {
        int x = i % xTiles;
        int y = i / xTiles;
        renderTexture(background, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
    }
}

/**
 * Check if we hit to another object
 * @param pos Coordinates of the object
 * @param size Size of texteru
 * @return true if hit else false
 */
bool checkCollision(const Position& pos, int size) {
    // Check X direction
    if (pos.x < 0 or pos.x + size > SCREEN_WIDTH)
        return true;

    // Check Y direction
    if (pos.y < 0 or pos.y + size > SCREEN_HEIGHT)
        return true;

    return false;
}