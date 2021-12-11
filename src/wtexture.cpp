#include "../headers/wtexture.h"
#include "../headers/main.h"

WTexture::WTexture() :
    texture(nullptr), size({0}), angle(0)
{
}

WTexture::WTexture(SDL_Texture* texture, int width, int height,  int x, int y, float angle, SDL_RendererFlip flip) :
    angle(angle)
{
    if (texture == nullptr)
        cerrErrorSDL("Texture wrapper constructor", "Input texture is a nullptr");

    this->texture = texture;
    
    size.w = width;
    size.h = height;
    size.x = x;
    size.y = y;
}

WTexture::WTexture(SDL_Texture* texture, SDL_Rect size, float angle, SDL_RendererFlip flip) :
    size(size), angle(angle)
{
    if (texture == nullptr)
        cerrErrorSDL("Texture wrapper constructor", "Input texture is a nullptr");

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
        size = {0};
    }
}

void WTexture::setWidth(int width) {
    size.w = width;
}

void WTexture::setHeigth(int height) {
    size.h = height;
}

const int WTexture::getWidth() {
    return size.w;
}

const int WTexture::getHeight() {
    return size.h;
}

void WTexture::setX(int x) {
    size.x = x;
}

void WTexture::setY(int y) {
    size.y = y;
}

const int WTexture::getX() {
    return size.x;
}

const int WTexture::getY() {
    return size.y;
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

/**
 * @brief Set rotation angle for the image
 * 
 * @param newAngle An angle value in degrees
 */
void WTexture::setAngle(const float& newAngle) {
    angle = newAngle;
}

float WTexture::getAngle() {
    return angle;
}