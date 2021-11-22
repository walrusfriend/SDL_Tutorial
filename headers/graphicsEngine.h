#pragma once

#include "main.h"
#include <map>

class GraphicsEngine {

public: 
    GraphicsEngine();
    ~GraphicsEngine();

    SDL_Texture* loadImage(std::string&& path);
    void renderTexture(SDL_Texture* texture, int x, int y, int w, int h);
    void renderTexture(SDL_Texture* texture, SDL_Rect dest, SDL_Rect* clip = nullptr);
    void renderTexture(SDL_Texture* texture, int x, int y, SDL_Rect* clip = nullptr);
    void renderBackground(SDL_Texture* background);
    SDL_Texture* renderText(const std::string& message, TTF_Font* font, SDL_Color color);

    void renderClear();
    void renderUpdate();

    bool addFont(const std::string& fontName, const int& fontSize);
    TTF_Font* getFont(const std::string& fontName);

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // Resource paths
    std::string imagesPath;
    std::string fontsPath;

    // Fonts
    std::map<std::string, TTF_Font*> fonts;
};