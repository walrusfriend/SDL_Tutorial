#pragma once

#include "main.h"

// Wrapper of the SDL_Texture
class WTexture
{
public:
    WTexture();
    WTexture(SDL_Texture* texture, int width, int height, int x, int y, 
             float angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);

    WTexture(SDL_Texture* texture, SDL_Rect size = {0}, float angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);
    ~WTexture();

    void clear();		// Delete image from memory

    void setWidth(int);
    void setHeigth(int);
    const int getWidth();
    const int getHeight();

    void setX(int);
    void setY(int);
    const int getX();
    const int getY();

    SDL_Texture* getTexture();
    void setTexture(SDL_Texture*);

    void setName(const std::string&);
    std::string getName();

    void setAngle(const float&);
    float getAngle();

private:
    SDL_Texture* texture;

    SDL_Rect size;

    float angle;

    std::string name;
};