#pragma once

#include <SDL.h>
#include "main.h"

// Wrapper of the SDL_Texture
class WTexture
{
public:
    WTexture();
    WTexture(SDL_Texture* texture, int width = 0, int height = 0);
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

private:
    SDL_Texture* texture;

    int width;
    int height;
    int x;
    int y;

    // TODO Implement a texture name (add path in gpx::loadImage())
    std::string name;
};