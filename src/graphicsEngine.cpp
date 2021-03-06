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
 * @brief Load texture from file
 * 
 * @param  path     The path to the file relative to the /image folder
 * @return          Returns texture or nullptr if error occurred
*/
WTexture* GraphicsEngine::loadImage(std::string&& path) {
    // Load image
    SDL_Texture* texture = IMG_LoadTexture(renderer, (imagesPath + path).c_str());
    if (!texture) {
        cerrErrorSDL("Load Image");
    }
    WTexture* wtext = new WTexture(texture);
    wtext->setName(path);
    textures.insert({path, wtext});
    return wtext;
}

/**
 * @brief Free memory from an image
 * 
 * @param path The texture file name with a format (*.png, etc)
 */
void GraphicsEngine::unloadImage(std::string&& path) {
    auto texture = textures.find(path);
    if (texture != textures.end()) {
        SDL_DestroyTexture(texture->second->getTexture());
        textures.erase(texture);
        delete texture->second;
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
WTexture* GraphicsEngine::getImage(const std::string& textureName) {
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
        SDL_DestroyTexture(i.second->getTexture());
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
void GraphicsEngine::renderTexture(WTexture& texture, int x, int y, int w, int h) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    if (SDL_RenderCopy(renderer, texture.getTexture(), NULL, &rect) != 0) {
        cerrErrorSDL("Render Texture");
    }
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
void GraphicsEngine::renderTexture(WTexture& texture, int x, int y, SDL_Rect* clip) {
    SDL_Rect target;
    target.x = x;
    target.y = y;
    if (clip != nullptr) {
        target.w = clip->w;
        target.h = clip->h;
    }
    else {
        SDL_QueryTexture(texture.getTexture(), nullptr, nullptr, &target.w, &target.h);
    }
    renderTexture(texture, target, clip);
}

/**
 * @brief image to target rect
 * 
 * @param texture       Pointer to the texture to draw
 * @param target        The rect in which you will draw
 * @param clip          A part of the image that will draw (nullptr will draw whole image)
 * @param flip          Set a vertical or horizontal reflection (None by default)
 * @param angle         Set a rotation angle in degrees
 * @param centerCoores  Point indicating the point around which image will be rotated
 * @return              None
 */
void GraphicsEngine::renderTexture(WTexture& texture, SDL_Rect dest, SDL_Rect* clip, SDL_RendererFlip flip, 
                                   float angle, SDL_Point* centerCoords)
{
    SDL_RenderCopyEx(renderer, texture.getTexture(), clip, &dest, angle, centerCoords, flip);
}

/**
 * Draw background
 * @param background    Pointer to image
 * @return              None
 */
void GraphicsEngine::renderBackground(WTexture& background) {
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
WTexture* GraphicsEngine::renderText(const std::string& message, WFont& font, SDL_Color color) {
    if (font.getFont() == nullptr)
        cerrErrorSDL("RenderText - font == nullptr");

    // Project text to the surface
    SDL_Surface* surface = TTF_RenderText_Blended(font.getFont(), message.c_str(), color);
    if (surface == nullptr)
        cerrErrorSDL("RenderText - couldn't project the font to the surface");

    // Transform surface to texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr)
        cerrErrorSDL("RenderText - failed when creating from the surface to the texture");

    // Set font texture size
    int fontWidth;
    int fontHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &fontWidth, &fontHeight);
    WTexture* wtext = new WTexture(texture, fontWidth, fontHeight, 0, 0);

    // Add texture to the texture map
    auto search = fonts.find(font.getName());
    if (search == fonts.end()) {
        cerrErrorSDL("renderText - font couldn't find");
    }
    textures.insert({message + font.getName(), wtext});

    // Free memory
    SDL_FreeSurface(surface);
    return wtext;
}

/**
 * @brief Clear the renderer
 */
void GraphicsEngine::renderClear() {
    SDL_RenderClear(renderer);
}

/**
 * @brief Update the renderer
 */
void GraphicsEngine::renderUpdate() {
    SDL_RenderPresent(renderer);
}

/**
 * @brief Add a font to the map with all fonts
 * 
 * @param fontName  The file name of the font with a format (*.ttf)
 * @param fontSize  The size of the font
 * @return A raw pointer to the font wrapper
 */
WFont* GraphicsEngine::addFont(const std::string& fontName, const int& fontSize) {
    TTF_Font* font = TTF_OpenFont((fontsPath + fontName).c_str(), fontSize);
    if (font == nullptr)
        cerrErrorSDL("Open font");

    WFont* wfont = new WFont(font, fontName);
    fonts.insert({fontName, wfont});
    return wfont;
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
        return search->second->getFont();
    }
    else {
        return nullptr;
    }
    
}

/**
 * @brief Delete a background from the texture
 * 
 * @param path Location of the texture relative to the "/images/" folder
 * @param R    Red component of the color (HEX)
 * @param G    Green component of the color (HEX)
 * @param B    Blue component of the color (HEX)
 * @return Pointer to the texture wrapper class (WTexture*)
 */
WTexture* GraphicsEngine::loadTextureWithoutBackground(const std::string& path, uint8_t R, uint8_t G, uint8_t B) {
    SDL_Surface* surf = IMG_Load((imagesPath + path).c_str());
    if (surf == nullptr)
        cerrErrorSDL("LoadTextureWithoutBackground - IMG_Load"); 

    SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 0xFF, 0xFF, 0xFF));

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (!texture)
        cerrErrorSDL("LoadTextureWithoutBackground - createTextureFromSurface");

    WTexture* wtext = new WTexture(texture);
    textures.insert({path, wtext});
    return wtext;
}