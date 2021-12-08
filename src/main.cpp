#include "../headers/main.h"
#include "../headers/application.h"

// Global variables
bool vscodeDebug = false;

// Function prototype
std::vector<std::string> parseApplicationInput(int argc, char** argv);

int main(int argc, char* argv[]) {

    // TODO Use "argc" and "argv" to setup debug mode or something else
    // Read the input and convert it to a normal types
    std::vector<std::string> applicationInput = std::move(parseApplicationInput(argc, argv));

    // Create the application instance
    std::unique_ptr<Application> app(new(Application));

    // Initialization
    if (app->init() == false)
        return 1;

    // Main game loop
    app->run();

    // Clear memory
    app->cleanup();
    return 0;
}

// TODO Finish this feature
std::vector<std::string> parseApplicationInput(int argc, char** argv) {
    std::vector<std::string> vec;

    for (int i = 1; i < argc; ++i) {
        vec.push_back(argv[i]);
    }

    auto result = std::find(vec.begin(), vec.end(), "--debug");
    if (result != vec.end()) {
        vscodeDebug = true;
    }
    return vec;
}

/**
 * @brief Create a message box and exit a program
 * 
 * @param str Error identificator string
 */
void cerrErrorSDL(std::string&& str) 
{ 
    std::cout << str + " Error: " << SDL_GetError() << std::endl;\
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                         (str + " error").c_str(),
                         SDL_GetError(),
                         NULL);
    std::_Exit(1);
}

/**
 * @brief Create a message box and exit a program
 * 
 * @param str Error identificator string
 * @param message Error description string
 */
void cerrErrorSDL(std::string&& str, std::string&& message) 
{ 
    std::cout << str + " Error: " << message << std::endl;\
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                         (str + " error").c_str(),
                         message.c_str(),
                         NULL);
    std::_Exit(1);
}