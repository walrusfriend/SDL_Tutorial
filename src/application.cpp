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
    std::unique_ptr<WFont> font(gpxEngine->addFont("font.ttf", 48));

    // Configure the font
    std::unique_ptr<WTexture> fontTexture(
        gpxEngine->renderText("Best Rogue Like ever!", *font, SDL_Color({255, 255, 255, 255})));

    fontTexture->setX((SCREEN_WIDTH - fontTexture->getWidth()) / 2);
    fontTexture->setY(10);

    // Set the cursor texture
    cursorTexture.reset(gpxEngine->loadImage("cursor.png"));
    SDL_Rect cursorSize = {0, 0, 32, 32};
    SDL_Rect cursorImagePart = {0, 0, 16, 16};
    SDL_ShowCursor(SDL_DISABLE);

    bool quit = false;
    SDL_Event event;
    keyboardState = SDL_GetKeyboardState(NULL);
    
    // Setup the timings
    double t = 0.0;
    double dt = 1/60.0;
    double currentTime = SDL_GetTicks() / 1000.0;     // Turn to seconds

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

                if (event.type == SDL_MOUSEMOTION) {
                    cursorSize.x = event.motion.x;
                    cursorSize.y = event.motion.y;
                }

                if (event.type == SDL_KEYDOWN) {
                    if (keyboardState[SDL_SCANCODE_W] or keyboardState[SDL_SCANCODE_S] or 
                        keyboardState[SDL_SCANCODE_A] or keyboardState[SDL_SCANCODE_D]) {
                        player->isMove = true;
                    }

                    if (keyboardState[SDL_SCANCODE_SPACE]) {
                        player->isAttack = true;
                    }
                }

                
            }

            // Characters movement
            player->moveDirection = definePlayerMovementDirection();
            player->attack();
            player->move(deltaTime);

            if (keyboardState[SDL_SCANCODE_ESCAPE])
                quit = true;

            frameTime -= deltaTime;
            t += deltaTime;
        }

        // Update the screen
        gpxEngine->renderClear();
        gpxEngine->renderBackground(*backgroundTile);
        gpxEngine->renderTexture(*fontTexture, fontTexture->getX(), fontTexture->getY());
        player->update();
        gpxEngine->renderTexture(*cursorTexture, cursorSize, &cursorImagePart);
        gpxEngine->renderUpdate();
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
    player->walkTextureSize = 16;
    player->attackTextureSize = 48;
    player->size.w = player->walkTextureSize;
    player->velocity = 250;

    // player texture states
    player->spriteInfo.numberOfStepInSprite = 3;
    player->spriteInfo.rotateLeft = 3;
    player->spriteInfo.rotateRight = 0;
    player->spriteInfo.rotateDown = -1;
    player->spriteInfo.rotateUp = -1;
    player->spriteInfo.clipsNumber = 6;
    player->spriteInfo.useWalkClip = 0;
    player->spriteInfo.moveFramerate = 100;
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

int Application::definePlayerMovementDirection() {
    if (keyboardState[SDL_SCANCODE_W] and keyboardState[SDL_SCANCODE_A]) {
        return Directions::UP_LEFT;
    }
    else if (keyboardState[SDL_SCANCODE_W] and keyboardState[SDL_SCANCODE_D]) {
        return Directions::UP_RIGHT;
    }
    else if (keyboardState[SDL_SCANCODE_S] and keyboardState[SDL_SCANCODE_A]) {
        return Directions::DOWN_LEFT;
    }
    else if (keyboardState[SDL_SCANCODE_S] and keyboardState[SDL_SCANCODE_D]) {
        return Directions::DOWN_RIGHT;
    }
    else if (keyboardState[SDL_SCANCODE_W]) {
        return Directions::UP;
    }
    else if (keyboardState[SDL_SCANCODE_S]) {
        return Directions::DOWN;
    }
    else if (keyboardState[SDL_SCANCODE_A]) {
        return Directions::LEFT;
    }
    else if (keyboardState[SDL_SCANCODE_D]) {
        return Directions::RIGHT;
    }
    else {
        return Directions::NONE;
    }
}