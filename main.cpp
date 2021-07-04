#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <yoga/Yoga.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <iomanip>
#include <malloc.h>

#include "src/engine.hpp"
#include "src/resource_manager.hpp"
#include "src/tiles.hpp"
#include "src/game_session.hpp"
#include "src/utils.hpp"
#include "src/game/live_photo.hpp"
#include "src/definitions/ship_definition.hpp"
#include "src/definitions/character_definition.hpp"
#include "src/definitions/star_system_definition.hpp"
#include "src/definitions/planet_surface_definition.hpp"
#include "src/definitions/item_definition.hpp"
#include "src/definitions/cursor.hpp"
#include "src/serialisers/json/json_serialisers_definitions.hpp"
#include "src/serialisers/json/json_serialisers_game.hpp"
#include "src/serialisers/json/json.hpp"
#include "src/definition_manager.hpp"
#include "src/game/ship.hpp"
#include "src/game/character.hpp"
#include "src/game/planet.hpp"
#include "src/game/star_system.hpp"
#include "src/game/planet_surface.hpp"
#include "src/game/items/chair.hpp"
#include "src/game/items/teleporter.hpp"
#include "src/game/particles_simple.hpp"
#include "src/keyboard.hpp"
#include "src/physics/polygon_collider.hpp"
#include "src/effects/transition.hpp"
#include "src/debug/draw_debug.hpp"
#include "src/controllers/npc_controller.hpp"
#include "src/ui/ui_manager.hpp"
#include "src/game-ui/game_ui_manager.hpp"
#include "src/mouse.hpp"
#define TRACK_MEMORY 1

#include <tmxlite/Map.hpp>

#include <X11/Xlib.h>

#ifdef TRACK_MEMORY
void doDelete(void *ptr)
{
    auto size = malloc_usable_size(ptr);
    space::DrawDebug::totalMemoryAllocated -= size;
    space::DrawDebug::freedThisFrame += size;

    free(ptr);
}

void operator delete(void *ptr, size_t origSize)
{
    doDelete(ptr);
}

void operator delete(void *ptr)
{
    doDelete(ptr);
}

void *operator new(size_t size)
{
    auto result = malloc(size);

    auto newSize = malloc_usable_size(result);
    space::DrawDebug::totalMemoryAllocated += newSize;
    space::DrawDebug::allocatedThisFrame += newSize;
    space::DrawDebug::numAllocations++;

    return result;
}
#endif

int errorHandler(Display *display, XErrorEvent *event)
{
    std::cout << "Error creating X Window" << std::endl;

    return 1;
}

void printTimeDifference(const char *prefix, std::chrono::system_clock::time_point time1, std::chrono::system_clock::time_point time2)
{
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1);
    std::cout << "Time taken for: " << prefix << " " << diff.count() << "ms" << std::endl;
}

int main()
{
    auto startTime = std::chrono::high_resolution_clock::now();

    sf::ContextSettings settings;
    settings.majorVersion = 3;
    settings.minorVersion = 0;
    settings.stencilBits = 1;

    XSetErrorHandler(errorHandler);

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Space Doggo", sf::Style::Default, settings);
    if (!window.setActive())
    {
        std::cout << "Failed starting window" << std::endl;
        return 1;
    }
    window.setVerticalSyncEnabled(true);
    // window.setFramerateLimit(120);

    auto afterCreateWindow = std::chrono::high_resolution_clock::now();

    glewInit();

    auto afterGlew = std::chrono::high_resolution_clock::now();

    space::Engine engine(&window);
    engine.spriteScale(4.0f);

    space::ResourceManager &resourceManager = engine.resourceManager();

    auto afterCreateEngine = std::chrono::high_resolution_clock::now();
    resourceManager.preloadTextures("data/textures");
    auto afterTextures = std::chrono::high_resolution_clock::now();
    resourceManager.preloadFonts("data/fonts");
    auto afterFonts = std::chrono::high_resolution_clock::now();
    resourceManager.preloadMaps("data/maps");
    auto afterMaps = std::chrono::high_resolution_clock::now();
    resourceManager.preloadSoundBuffers("data/sounds");
    auto afterSounds = std::chrono::high_resolution_clock::now();

    auto &definitionManager = engine.definitionManager();
    definitionManager.loadFolder("data/definitions");

    definitionManager.onPostLoad(engine);
    auto afterDefinitions = std::chrono::high_resolution_clock::now();

    printTimeDifference("Window", startTime, afterCreateWindow);
    printTimeDifference("GLEW", afterCreateWindow, afterGlew);
    printTimeDifference("Engine", afterGlew, afterCreateEngine);
    printTimeDifference("Textures", afterCreateEngine, afterTextures);
    printTimeDifference("Fonts", afterTextures, afterFonts);
    printTimeDifference("Maps", afterFonts, afterMaps);
    printTimeDifference("Sounds", afterMaps, afterSounds);
    printTimeDifference("Definitions", afterSounds, afterDefinitions);

    engine.initEffects();
    engine.uiManager().initDefaultWindows();

    engine.changeCursor("CURSOR_DEFAULT");

    std::ifstream startingGameFile("data/startingGame.json");
    nlohmann::json startingGameJson;
    startingGameFile >> startingGameJson;

    // auto gameSessionTemp = space::fromJsonGameSession(engine, startingGameJson);
    // auto gameSession = gameSessionTemp.get();
    // engine.currentSession(std::move(gameSessionTemp));

    auto &gameUIManager = engine.gameUIManager();

    gameUIManager.defaultFont(resourceManager.font("data/fonts/PixelOperator.ttf"));
    gameUIManager.defaultWindowBackPanel(space::NineSlice(resourceManager.texture("data/textures/testPanel3.png"), 30, 8, 8, 8));
    gameUIManager.defaultPanelBackPanel(space::NineSlice(resourceManager.texture("data/textures/testPanel4.png"), 8, 8, 8, 8));
    gameUIManager.defaultButton(space::NineSlice(resourceManager.texture("data/textures/testButton.png"), 8, 8, 8, 8));

    gameUIManager.initDefaultWindows();

    // auto particles = gameSession->createObject<space::ParticlesSimple>("PARTICLES_1", 10000);
    // particles->transform().position = sf::Vector2f(-150.0f, 0);
    // starSystem1->area().addObject(particles);

    auto endTime = std::chrono::high_resolution_clock::now();

    printTimeDifference("Overall startup", startTime, endTime);

    while (window.isOpen())
    {
        space::DrawDebug::allocatedThisFrame = 0;
        space::DrawDebug::freedThisFrame = 0;
        space::DrawDebug::locksUsed = 0;
        space::DrawDebug::numAllocations = 0;

        engine.processEvents();
        engine.preUpdate();

        engine.update();
        engine.draw();

        if (space::DrawDebug::slow)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
    }

    engine.shutdown();

    return 0;
}
