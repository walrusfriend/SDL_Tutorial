#pragma once

#include <memory>

#include <SDL.h>
#include <SDL_ttf.h>
#include "../headers/wtexture.h"

// TODO Read the ttf's docs
// Wrapper of the SDL_Texture
class WFont
{
public:
    WFont();
    WFont(TTF_Font*, const std::string&);
    ~WFont();

    void clear();

    void setFontSize(int);
    const int getFontSize();

    TTF_Font* getFont();
    void setFont(TTF_Font*);

    void setName(const std::string&);
    std::string getName();

private:
    TTF_Font* font;
    
    std::string name;
    int fontSize;
};