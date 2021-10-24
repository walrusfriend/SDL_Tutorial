#include "../headers/main.h"
#include "../headers/character.h"

// Set tile size
const int TILE_SIZE = 40;

// Set path to images folder
std::string imagesPath = "images/";
std::string fontsPath = "fonts/";

// If debug from vs code
// #define DEBUG_VS_CODE

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

// Function defenition
bool init();
SDL_Texture* loadImage(std::string&& path);
void renderTexture(SDL_Texture* texture, int x, int y, int w, int h);
void renderTexture(SDL_Texture* texture, SDL_Rect dest, SDL_Rect* clip = nullptr);
void renderTexture(SDL_Texture* texture, int x, int y, SDL_Rect* clip = nullptr);
void cleanup();

void renderBackground(SDL_Texture* background);
bool checkCollisionWithBorders(const Character& pers);
SDL_Texture* renderText(const std::string& message, TTF_Font* font, SDL_Color color);
void cerrErrorSDL(std::string&& str) { std::cout << str + " Error: " << SDL_GetError() << std::endl; }

int main(int argc, char** argv) {

    if (init() == false)
        return 1;

    // Create window
    window = SDL_CreateWindow("SDL_Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        cerrErrorSDL("Create Window");
        return 1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        cerrErrorSDL("Create Renderer");
        return 1;
    }

#ifdef DEBUG_VS_CODE
    imagesPath = "../images/";
    fontsPath = "../fonts/";
#endif

    // Chatacter
    std::unique_ptr<Character> person(new Character);
    person->width = 100;
    person->height = 100;
    int x = (SCREEN_WIDTH - person->width) / 2;
    int y = (SCREEN_HEIGHT - person->height) / 2;
    person->x = x;
    person->y = y;
    person->stepSize = 10;

    // Load texture
    SDL_Texture* background = nullptr;

    background = loadImage("background.png");
    person->texture = loadImage("ball.png");
    if (!background || !person->texture) {
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(person->texture);
        cleanup();
        return 1;
    }
    
    SDL_Rect clips[4];
    for (int i = 0; i < 4; ++i) {
        clips[i].x = i / 2 * person->width;
        clips[i].y = i % 2 * person->height;
        clips[i].w = person->width;
        clips[i].h = person->height;
    }

    int useClip = 0;

    // Setting the font
    SDL_Color fontColor = {255, 255, 255, 255};
    TTF_Font* font = TTF_OpenFont((fontsPath + "sample.ttf").c_str(), 64);
    if (font == nullptr) {
        cerrErrorSDL("Open font");
        return 1;
    }

    SDL_Texture* textImage = renderText("TTF fonts are cool!", font, fontColor);
    if (textImage == nullptr) {
        cleanup();
    }

    int fontWidth;
    int fontHeight;
    SDL_QueryTexture(textImage, nullptr, nullptr, &fontWidth, &fontHeight);
    int fontX = (SCREEN_WIDTH - fontWidth) / 2;
    int fontY = 0;

    bool quit = false;
    SDL_Event event;

    // main loop
    while (!quit) {
        // Event processing
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN) {
                // TODO When a key is hold figure must move without interruptions
                const uint8_t* state = SDL_GetKeyboardState(NULL);
                // Diagional movement
                if (state[SDL_SCANCODE_1])
                    useClip = 0;
                else if (state[SDL_SCANCODE_2])
                    useClip = 1;
                else if (state[SDL_SCANCODE_3])
                    useClip = 2;
                else if (state[SDL_SCANCODE_4])
                    useClip = 3;
                else if (state[SDL_SCANCODE_W] and state[SDL_SCANCODE_A])
                    person->move(UP_LEFT);
                    // charMove(charPos, charSize, charStepSize, UP_LEFT);
                else if (state[SDL_SCANCODE_W] and state[SDL_SCANCODE_D])
                    person->move(UP_RIGHT);
                    // charMove(charPos, charSize, charStepSize, UP_RIGHT);
                else if (state[SDL_SCANCODE_S] and state[SDL_SCANCODE_A])
                    person->move(DOWN_LEFT);
                    // charMove(charPos, charSize, charStepSize, DOWN_LEFT);
                else if (state[SDL_SCANCODE_S] and state[SDL_SCANCODE_D])
                    person->move(DOWN_RIGHT);
                    // charMove(charPos, charSize, charStepSize, DOWN_RIGHT);
                else if (state[SDL_SCANCODE_W])
                    person->move(UP);
                    // charMove(charPos, charSize, charStepSize, UP);
                else if (state[SDL_SCANCODE_S])
                    person->move(DOWN);
                    // charMove(charPos, charSize, charStepSize, DOWN);
                else if (state[SDL_SCANCODE_A])
                    person->move(LEFT);
                    // charMove(charPos, charSize, charStepSize, LEFT);
                else if (state[SDL_SCANCODE_D])
                    person->move(RIGHT);
                    // charMove(charPos, charSize, charStepSize, RIGHT);
                else if (state[SDL_SCANCODE_ESCAPE])
                    quit = true;
            }
        }

        // Clear window
        SDL_RenderClear(renderer);
        renderBackground(background);
        renderTexture(textImage, fontX, fontY);
        renderTexture(person->texture, person->x, person->y, &clips[useClip]);
        SDL_RenderPresent(renderer);

        // Free CPU resources
        SDL_Delay(1);
    }

    cleanup();

    return 0;
}

/**
 * Initialization of SDL and its components
 * @return Returns true if all right else return false
 */
bool init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        cerrErrorSDL("SDL_Init");
        return false;
    }
    
    // Initialize SDL_Image module
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        cerrErrorSDL("IMG_Init");
        return false;
    }

    // Initialize TTF module
    if (TTF_Init() != 0) {
        cerrErrorSDL("TTF module");
        return false;
    }
    return true;
}

/**
 * Load texture from file on the path "path"
 * @param  path     Path to the file
 * @return          Returns texture or nullptr if error occurred
*/
SDL_Texture* loadImage(std::string&& path) {
    // Load image
    SDL_Texture* texture = IMG_LoadTexture(renderer, (imagesPath + path).c_str());
    if (!texture) {
        cerrErrorSDL("Load Image");
    }
    return texture;
}

/**
 * Draw image on renderer with scaling
 * @param texture   Pointer to the texture to draw
 * @param x         X coordinate of the drawing location
 * @param y         Y coordinate of the drawing location
 * @param w         Scaling width
 * @param h         Scaling height
 * @return          None
 */
void renderTexture(SDL_Texture* texture, int x, int y, int w, int h) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    if (SDL_RenderCopy(renderer, texture, NULL, &rect) != 0) {
        cerrErrorSDL("Render Texture");
    }
}

/**
 * Draw image to target rect
 * @param texture   Pointer to the texture to draw
 * @param target    The rect in which you will draw
 * @param clip      A part of the image that will draw (nullptr will draw whole image)
 * @return          None
 */
void renderTexture(SDL_Texture* texture, SDL_Rect target, SDL_Rect* clip) {
    SDL_RenderCopy(renderer, texture, clip, &target);
}

/**
 * Draw original image or its part to certain point (x,y)
 * If you draw the whole image, its size will be used
 * If you draw only a part of the image, the size of the part will be used
 * @param texture   Pointer to the texture to draw
 * @param x         X coordinate of the drawing location
 * @param y         Y coordinate of the drawing location
 * @param clip      A part of the image that will draw
 * @return          None
 */
void renderTexture(SDL_Texture* texture, int x, int y, SDL_Rect* clip) {
    SDL_Rect target;
    target.x = x;
    target.y = y;
    if (clip != nullptr) {
        target.w = clip->w;
        target.h = clip->h;
    }
    else {
        SDL_QueryTexture(texture, nullptr, nullptr, &target.w, &target.h);
    }
    renderTexture(texture, target, clip);
}

/**
 * Free resources
 * @return None
 */
void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

/**
 * Draw background
 * @param background    Pointer to image
 * @return              None
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
 * @param pos   Coordinates of the object
 * @param size  Size of texture
 * @return      True if hit else false
 */
/*bool checkCollision(const Position& pos, int size) {
    // Check X direction
    if (pos.x < 0 or pos.x + size > SCREEN_WIDTH)
        return true;

    // Check Y direction
    if (pos.y < 0 or pos.y + size > SCREEN_HEIGHT)
        return true;

    return false;
}*/

/*void charMove(Position& charPos, int charSize, int charStepSize, int direction) {
    Position nextPos = {charPos.x, charPos.y};

    // When we step on diagonal direction, an object move at a distance of hypotenuse
    // Therefore, need to define the value of X and Y components
    Position diagPos = nextPos;
    float rootOf2 = 1.41421356237f;                 // The root of 2 required to calculate the diagonal distance
    int diagStepSize = charStepSize / rootOf2;      // We must convert from float to int
    switch (direction) {
        case UP:
            nextPos.y -= charStepSize;
            if (!checkCollision(nextPos, charSize))
                charPos = nextPos;
            break;
        case DOWN:
            nextPos.y += charStepSize;
            if (!checkCollision(nextPos, charSize))
                charPos = nextPos;
            break;
        case LEFT:
            nextPos.x -= charStepSize;
            if (!checkCollision(nextPos, charSize))
                charPos = nextPos;
            break;
        case RIGHT:
            nextPos.x += charStepSize;
            if (!checkCollision(nextPos, charSize))
                charPos = nextPos;
            break;
        case UP_LEFT:
            charMove(charPos, charSize, diagStepSize, UP);
            charMove(charPos, charSize, diagStepSize, LEFT);
            break;
        case UP_RIGHT:
            charMove(charPos, charSize, diagStepSize, UP);
            charMove(charPos, charSize, diagStepSize, RIGHT);
            break;
        case DOWN_LEFT:
            charMove(charPos, charSize, diagStepSize, DOWN);
            charMove(charPos, charSize, diagStepSize, LEFT);
            break;
        case DOWN_RIGHT:
            charMove(charPos, charSize, diagStepSize, DOWN);
            charMove(charPos, charSize, diagStepSize, RIGHT);
            break;
        default:
            std::cerr << "ERROR: Character movement error!" << std::endl;
            break;
    }
}*/

/**
 * Draw a text to the screen
 * @param message   Text to be written
 * @param font      Pointer to TTF_Font object
 * @param color     Color of the text
 * @param fontSize  Size of the text
 * @return          Pointer to the texture or nullptr if something went wrong
 */
SDL_Texture* renderText(const std::string& message, TTF_Font* font, SDL_Color color) {
    if (font == nullptr) {
        std::cerr << "RenderText Error: Empty font file!" << std::endl;
        return nullptr;
    }

    // Project text to the surface
    SDL_Surface* surface = TTF_RenderText_Blended(font, message.c_str(), color);
    if (surface == nullptr) {
        cerrErrorSDL("TTF_RenderText");
        return nullptr;
    }

    // Transform surface to texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        cerrErrorSDL("Texture creation");
    }

    // Free memory
    SDL_FreeSurface(surface);
    return texture;
}