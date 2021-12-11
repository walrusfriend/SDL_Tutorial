#pragma once

#include <map>

#include "main.h"
#include "character.h"
#include "wtexture.h"
#include "wfont.h"

static SDL_Renderer* renderer;

class GraphicsEngine {

public: 
    GraphicsEngine();
    ~GraphicsEngine();

    WTexture* loadImage(std::string&& path);
    void unloadImage(std::string&& path);
    WTexture* getImage(const std::string& textureName);
    void destroyAllTextures();
    static void renderTexture(WTexture& texture, int x, int y, int w, int h);
    static void renderTexture(WTexture& texture, int x, int y, SDL_Rect* clip = nullptr);
    static void renderTexture(WTexture& texture, SDL_Rect dest, SDL_Rect* clip = nullptr, 
                              SDL_RendererFlip flip = SDL_FLIP_NONE, float angle = 0, SDL_Point* centerCoords = nullptr);
    void renderBackground(WTexture& background);
    WTexture* renderText(const std::string& message, WFont& fontName, SDL_Color color);

    void renderClear();
    void renderUpdate();

    WFont* addFont(const std::string& fontName, const int& fontSize);
    TTF_Font* getFont(const std::string& fontName);

    WTexture* loadTextureWithoutBackground(const std::string& path, uint8_t R, uint8_t G, uint8_t B);

private:
    SDL_Window* window = nullptr;

    // Resource paths
    std::string imagesPath;
    std::string fontsPath;

    // Fonts
    std::map<std::string, WFont*> fonts;

    // Textures
    std::map<std::string, WTexture*> textures;
};