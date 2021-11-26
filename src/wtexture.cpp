#include "../headers/wtexture.h"

WTexture::WTexture() :
    texture(nullptr), width(0), height(0)
{
}

WTexture::WTexture(SDL_Texture* texture, int width, int height) :
    texture(texture), width(width), height(height)
{
    this->texture = texture;
}

WTexture::~WTexture() {
    clear();
}

/**
 * @brief Deallocate a memory
 */
void WTexture::clear() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
        width = 0;
        height = 0;
    }
}

void WTexture::setWidth(int newWidth) {
    width = newWidth;
}

void WTexture::setHeigth(int newHeight) {
    height = newHeight;
}

const int WTexture::getWidth() {
    return width;
}

const int WTexture::getHeight() {
    return height;
}

SDL_Texture* WTexture::getTexture() {
    if (texture == nullptr)
        return nullptr;
    return texture;
}

void WTexture::setTexture(SDL_Texture* newTexture) {
    texture = newTexture;
}