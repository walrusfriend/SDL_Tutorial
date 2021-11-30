#include "../headers/application.h"
 
Application::Application() {
}

Application::~Application () {
}

/**
 * @brief Initialization of SDL and its modules
 * 
 * @return true if OK
 * @return false if not OK
 */
bool Application::init() {
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
 * @brief Main game loop
 * 
 */
void Application::run() {
    gpxEngine.reset(new GraphicsEngine);

    // Chatacter
    player.reset(new Character);
    playerSetup(player);

    // Load texture
    std::unique_ptr<WTexture> backgroundTile(gpxEngine->loadImage("basictiles.png"));

    // Delete background
    player->texture.reset(gpxEngine->loadTextureWithoutBackground("player/swordman_white_back.png", 0xFF, 0xFF, 0xFF));
    
    // Setting the font
    gpxEngine->addFont("font.ttf", 48);

    SDL_Color fontColor = {255, 255, 255, 255};
    std::unique_ptr<WTexture> textImage(new WTexture());
    textImage->setTexture(gpxEngine->renderText("Best Rogue Like ever!", gpxEngine->getFont("font.ttf"), fontColor));
    if (textImage->getTexture() == nullptr) {
        gpxEngine->destroyAllTextures();
        cleanup();
        return;
    }

    int fontWidth;
    int fontHeight;
    SDL_QueryTexture(textImage->getTexture(), nullptr, nullptr, &fontWidth, &fontHeight);
    textImage->setWidth(fontWidth);
    textImage->setHeigth(fontHeight);
    int fontX = (SCREEN_WIDTH - fontWidth) / 2;
    int fontY = 10;

    bool quit = false;
    SDL_Event event;
    const uint8_t* state = SDL_GetKeyboardState(NULL);
    
    // Setup the timings
    double t = 0.0;
    double dt = 1/60.0;
    double currentTime = SDL_GetTicks() / 1000.0;     // Turn to seconds

    // TODO Delete this
    int frame = 0;

    while (!quit) {
        double newTime = SDL_GetTicks() / 1000.0;     // Turn to seconds
        double frameTime = newTime - currentTime;
        currentTime = newTime;

        while(frameTime > 0.0) {
            double deltaTime = std::min(frameTime, dt);

            // Event processing
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }

                if (event.type == SDL_KEYDOWN) {
                    if (state[SDL_SCANCODE_W] or state[SDL_SCANCODE_S] or state[SDL_SCANCODE_A] or state[SDL_SCANCODE_D])
                        player->isMove = true;
                    else
                        player->isMove = false;
                }
            }

            player->move(deltaTime);

            if (state[SDL_SCANCODE_ESCAPE])
                quit = true;

            frameTime -= deltaTime;
            t += deltaTime;
        }

        // Clear window
        gpxEngine->renderClear();
        gpxEngine->renderBackground(*backgroundTile);
        gpxEngine->renderTexture(*textImage, fontX, fontY);
        gpxEngine->renderTexture(*player->texture, player->size, 
                                &player->spriteInfo.walkSprite[frame / 4]);
        gpxEngine->renderUpdate();

        if (player->isMove) {
            ++frame;
            if (frame / 4 >= 3) {
                frame = 0;
            }
        }
        else {
            frame = 0;
        }
    }
}

/**
 * Free resources
 * @return None
 */
void Application::cleanup() {
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

/**
 * @brief Setup start player settings (TEMPORARY)
 * 
 * @param player Reference to player that will be cofigured
 */
void Application::playerSetup(std::unique_ptr<Character>& player) {
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