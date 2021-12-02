#include "../headers/wtexture.h"
#include "../headers/main.h"

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

void WTexture::setX(int newX) {
    x = newX;
}

void WTexture::setY(int newY) {
    y = newY;
}

const int WTexture::getX() {
    return x;
}

const int WTexture::getY() {
    return y;
}

SDL_Texture* WTexture::getTexture() {
    return texture;
}

void WTexture::setTexture(SDL_Texture* newTexture) {
    if (newTexture == nullptr) {
        cerrErrorSDL("Texture::setTexture - a new texture points to a nullptr");
    }
    
    clear();
    texture = newTexture;
}

void WTexture::setName(const std::string& newName) {
    name = newName;
}

std::string WTexture::getName() {
    return name;
}