#include "../headers/main.h"
#include "../headers/character.h"
#include "../headers/graphicsEngine.h"

std::string fontsPath = "fonts/";

// Function defenition
bool init();
void cleanup();
void playerSetup(std::unique_ptr<Character>& player);

// Global variables
std::unique_ptr<GraphicsEngine> gpxEngine;

int main(int argc, char** argv) {

    // TODO Use "argc" and "argv" to setup debug mode or something else

    if (init() == false)
        return 1;

    gpxEngine.reset(new GraphicsEngine);

#ifdef DEBUG_VS_CODE
    fontsPath = "../fonts/";
#endif

    // Chatacter
    std::unique_ptr<Character> player(new Character);
    playerSetup(player);

    // Load texture
    SDL_Texture* backgroundTile = nullptr;

    backgroundTile = gpxEngine->loadImage("basictiles.png");
    player->texture = gpxEngine->loadImage("player/swordman.png");
    if (!backgroundTile || !player->texture) {
        SDL_DestroyTexture(backgroundTile);
        SDL_DestroyTexture(player->texture);
        cleanup();
        return 1;
    }
    
    // Setting the font
    SDL_Color fontColor = {255, 255, 255, 255};
    TTF_Font* font = TTF_OpenFont((fontsPath + "font.ttf").c_str(), 48);
    if (font == nullptr) {
        cerrErrorSDL("Open font");
        return 1;
    }

    SDL_Texture* textImage = gpxEngine->renderText("Best Rogue Like ever!", font, fontColor);
    if (textImage == nullptr) {
        cleanup();
        return 1;
    }

    int fontWidth;
    int fontHeight;
    SDL_QueryTexture(textImage, nullptr, nullptr, &fontWidth, &fontHeight);
    int fontX = (SCREEN_WIDTH - fontWidth) / 2;
    int fontY = 10;

    bool quit = false;
    SDL_Event event;

    // Setup the timings
    double t = 0.0;
    double dt = 1/60.0;

    double currentTime = SDL_GetTicks();

    const uint8_t* state = SDL_GetKeyboardState(NULL);

    // main loop
    while (!quit) {
        double newTime = SDL_GetTicks();
        double frameTime = newTime - currentTime;
        currentTime = newTime;

        // TODO Почему персонаж останавливается? Почему не работает закоменченный цикл ниже?
        // while (frameTime > 0.0) {
            double deltaTime = std::min(frameTime, dt);

            // Event processing
            while(SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }
            }
                // if (event.type == SDL_KEYDOWN) {
                    if (state[SDL_SCANCODE_W] or state[SDL_SCANCODE_S] or state[SDL_SCANCODE_A] or state[SDL_SCANCODE_D])
                        player->isMove = true;
                    // else
                    //     player->isMove = false;
                // }

                if (player->isMove) {
                    if (state[SDL_SCANCODE_W] and state[SDL_SCANCODE_A])
                        player->move(UP_LEFT, deltaTime);
                    else if (state[SDL_SCANCODE_W] and state[SDL_SCANCODE_D])
                        player->move(UP_RIGHT, deltaTime);
                    else if (state[SDL_SCANCODE_S] and state[SDL_SCANCODE_A])
                        player->move(DOWN_LEFT, deltaTime);
                    else if (state[SDL_SCANCODE_S] and state[SDL_SCANCODE_D])
                        player->move(DOWN_RIGHT, deltaTime);
                    else if (state[SDL_SCANCODE_W])
                        player->move(UP, deltaTime);
                    else if (state[SDL_SCANCODE_S])
                        player->move(DOWN, deltaTime);
                    else if (state[SDL_SCANCODE_A])
                        player->move(LEFT, deltaTime);
                    else if (state[SDL_SCANCODE_D])
                        player->move(RIGHT, deltaTime);
                }   

                if (state[SDL_SCANCODE_ESCAPE])
                    quit = true;

                // If we release the key the player will have to stop
                // If the manipulating key still pressed when player stops
                // in the moment of one cycle and then will start again
                // if (event.type == SDL_KEYUP) {
                //     // if (!(state[SDL_SCANCODE_W] or state[SDL_SCANCODE_S] or state[SDL_SCANCODE_A] or state[SDL_SCANCODE_D])) {
                //         player->isMove = false;
                //         if (player->velocityX != 0) {
                //             player->velocityX = 0;
                //         }

                //         if (player->velocityY != 0) {
                //             player->velocityY = 0;
                //         }
                //     // }
                // }

            frameTime -= deltaTime;
            t += deltaTime;
        // }

        // Clear window
        gpxEngine->renderClear();
        gpxEngine->renderBackground(backgroundTile);
        gpxEngine->renderTexture(textImage, fontX, fontY);
        gpxEngine->renderTexture(player->texture, player->size, &player->spriteInfo.walkSprite[player->spriteInfo.useWalkClip]);
        gpxEngine->renderUpdate();

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
 * Free resources
 * @return None
 */
void cleanup() {
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

/**
 * Send SDL error string to std::cerr stream
 */
void cerrErrorSDL(std::string&& str) 
{ 
    std::cout << str + " Error: " << SDL_GetError() << std::endl; 
}

void playerSetup(std::unique_ptr<Character>& player) {
    // Normalize the size of the character to 16-pixel lenght
    // Original pixture size is 16x26 when character walk and 56x26 when attack
    player->size.h = 26;
    int x = (SCREEN_WIDTH - player->size.w) / 2;
    int y = (SCREEN_HEIGHT - player->size.h) / 2;
    player->size.x = x;
    player->size.y = y;
    player->stepSize = TILE_SIZE;
    player->walkTextureSize = 16;
    player->attackTextureSize = 48;
    player->size.w = player->walkTextureSize;

    // player texture states
    player->spriteInfo.numberOfStepInSprite = 3;
    player->spriteInfo.rotateLeft = 3;
    player->spriteInfo.roteteRight = 0;
    player->spriteInfo.rotateDown = -1;
    player->spriteInfo.rotateUp = -1;
    player->spriteInfo.clipsNumber = 6;
    player->spriteInfo.useWalkClip = 0;
    for (int i = 0; i < player->spriteInfo.clipsNumber; ++i) {
        SDL_Rect tempRect;
        tempRect.x = i % player->spriteInfo.numberOfStepInSprite * player->size.w;
        tempRect.y = i / player->spriteInfo.numberOfStepInSprite * player->size.h;
        tempRect.w = player->walkTextureSize;
        tempRect.h = player->size.h;
        player->spriteInfo.walkSprite.push_back(tempRect);
    }

    player->scale(ZOOM);
}