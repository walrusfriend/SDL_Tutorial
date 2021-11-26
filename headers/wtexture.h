#pragma once

#include <SDL.h>

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

    SDL_Texture* getTexture();
    void setTexture(SDL_Texture*);

private:
    SDL_Texture* texture;

    int width;
    int height;
};