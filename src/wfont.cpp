#include "../headers/wfont.h"
#include "../headers/main.h"

WFont::WFont() {
    font = nullptr;
    name = "";
    fontSize = 0;
}

WFont::WFont(TTF_Font* newFont, const std::string& newName) {
    if (newFont == nullptr) {
        cerrErrorSDL("WFont constructor - new font points to a nullptr");
    }
    font = newFont;
    name = newName;
}

WFont::~WFont() {
    clear();
}

/**
 * @brief Free a memory from raw pointers
 * 
 */
void WFont::clear() {
    if (font != nullptr) {
        TTF_CloseFont(font);
    }
}

void WFont::setFontSize(int newFontSize) {
    if (newFontSize <= 0)
        cerrErrorSDL("setFontSize - the font size cann't be lower that 0");
    fontSize = newFontSize;
}

const int WFont::getFontSize() {
    return fontSize;
}

TTF_Font* WFont::getFont() {
    return font;
}

void WFont::setFont(TTF_Font* newFont) {
    if (newFont == nullptr) {
        cerrErrorSDL("setFont - new font is a nullptr");
    }
        
    clear();
    font = newFont;
}

void WFont::setName(const std::string& newName) {
    name = newName;
}

std::string WFont::getName() {
    return name;
}