#include "../headers/graphicsEngine.h"

GraphicsEngine::GraphicsEngine() {
    // Create window
    window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
        cerrErrorSDL("Create Window");

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
        cerrErrorSDL("Create Renderer");

    // Set path to images folder
    if (vscodeDebug) {
        imagesPath = "../images/";
        fontsPath = "../fonts/";
    }
    else {
        imagesPath = "images/";
        fontsPath = "fonts/";
    }
}

GraphicsEngine::~GraphicsEngine() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

/**
 * @brief Load texture from file on the path "path" to texture map
 * 
 * @param  path     Path to the file
 * @return          Returns texture or nullptr if error occurred
*/
SDL_Texture* GraphicsEngine::loadImage(std::string&& path) {
    // Load image
    SDL_Texture* texture = IMG_LoadTexture(renderer, (imagesPath + path).c_str());
    if (!texture) {
        cerrErrorSDL("Load Image");
        return texture;
    }
    textures.insert({path, texture});
    return texture;
}

/**
 * @brief Free memory from an image
 * 
 * @param texture The texture file name with a format (*.png, etc)
 */
void GraphicsEngine::unloadImage(std::string&& path) {
    auto texture = textures.find(path);
    if (texture != textures.end()) {
        SDL_DestroyTexture(texture->second);
        textures.erase(texture);
    }
    else {
        std::cerr << "Can't unload image: The texture with that name." << std::endl;
    }
}

/**
 * @brief Get the texture from the texture map by the name
 * 
 * @param textureName   The texture file name with a format (*.png, etc.)
 * @return SDL_Texture  Pointer to the texture or nullptr if it doesn't exist
 */
SDL_Texture* GraphicsEngine::getImage(const std::string& textureName) {
    auto search = textures.find(textureName);
    if (search != textures.end()) {
        return search->second;
    }
    else {
        std::cerr << "Can't get texture: The texture with that name does not exist." << std::endl;
        return nullptr;
    }
}

/**
 * @brief Free memory from all textures
 * 
 */
void GraphicsEngine::destroyAllTextures() {
    for (auto i : textures) {
        SDL_DestroyTexture(i.second);
    }
    textures.clear();
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
void GraphicsEngine::renderTexture(SDL_Texture* texture, int x, int y, int w, int h) {
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
void GraphicsEngine::renderTexture(SDL_Texture* texture, SDL_Rect target, SDL_Rect* clip) {
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
void GraphicsEngine::renderTexture(SDL_Texture* texture, int x, int y, SDL_Rect* clip) {
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
 * Draw background
 * @param background    Pointer to image
 * @return              None
 */
void GraphicsEngine::renderBackground(SDL_Texture* background) {
    int xTiles = SCREEN_WIDTH / (ZOOM * TILE_SIZE);
    int yTiles = SCREEN_HEIGHT / (ZOOM * TILE_SIZE);

    // Choose a tile from tile set
    SDL_Rect tile;
    tile.x = 0 * TILE_SIZE;
    tile.y = 2 * TILE_SIZE;
    tile.w = TILE_SIZE;
    tile.h = TILE_SIZE;

    // Set a rect to draw
    SDL_Rect target;
    target.w = ZOOM * TILE_SIZE;
    target.h = ZOOM * TILE_SIZE;

    for (int i = 0; i < xTiles * yTiles; ++i) {
        target.x = i % xTiles * ZOOM * TILE_SIZE;
        target.y = i / xTiles * ZOOM * TILE_SIZE;
        // renderTexture(background,  ZOOM * x * TILE_SIZE, ZOOM * y * TILE_SIZE, &tile);
        renderTexture(background, target, &tile);
    }
}

/**
 * Draw a text to the screen
 * @param message   Text to be written
 * @param font      Pointer to TTF_Font object
 * @param color     Color of the text
 * @param fontSize  Size of the text
 * @return          Pointer to the texture or nullptr if something went wrong
 */
SDL_Texture* GraphicsEngine::renderText(const std::string& message, TTF_Font* font, SDL_Color color) {
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

/**
 * Clear the renderer
 */
void GraphicsEngine::renderClear() {
    SDL_RenderClear(renderer);
}

/**
 * Update the renderer
 */
void GraphicsEngine::renderUpdate() {
    SDL_RenderPresent(renderer);
}

/**
 * @brief Add a font to the map with all fonts
 * 
 * @param fontName  The file name of the font with a format (*.ttf)
 * @param fontSize  The size of the font
 * @return true     If all font is normally loaded
 * @return false    If coundn't open the font
 */
bool GraphicsEngine::addFont(const std::string& fontName, const int& fontSize) {
    TTF_Font* font = TTF_OpenFont((fontsPath + fontName).c_str(), fontSize);
    if (font == nullptr) {
        cerrErrorSDL("Open font");
        return false;
    }
    fonts.insert({fontName, font});
    return true;
}

/**
 * @brief Get the font from the map with all font by the name
 * 
 * @param fontName      The file name of the font with a format (*.ttf)
 * @return TTF_Font*    Pointer to TTF-file with font or nullptr if font doesn't exist
 */
TTF_Font* GraphicsEngine::getFont(const std::string& fontName) {
    auto search = fonts.find(fontName);
    if (search != fonts.end()) {
        return search->second;
    }
    else {
        return nullptr;
    }
    
}


void GraphicsEngine::drawSprite(const Character& person) {
    
}