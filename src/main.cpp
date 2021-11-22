#include "../headers/main.h"
#include "../headers/application.h"

// Function prototype
std::vector<std::string> parseApplicationInput(int argc, char** argv);

int main(int argc, char** argv) {

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
    vscodeDebug = false;
    return vec;
}

/**
 * @brief Send SDL error string to std::cerr stream
 */
void cerrErrorSDL(std::string&& str) 
{ 
    std::cout << str + " Error: " << SDL_GetError() << std::endl; 
}