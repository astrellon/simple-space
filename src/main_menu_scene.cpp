#include "main_menu_scene.hpp"

#include "engine.hpp"
#include "render_camera.hpp"

namespace space
{
    MainMenuScene::MainMenuScene(Engine &engine):
        _engine(engine)
    {
        StarBackgroundOptions *options = new StarBackgroundOptions();
        options->backgroundColour = sf::Color(0x121428FF);
        options->onPostLoad(engine);
        _background = std::make_unique<StarBackground>(engine, *options);
    }

    void MainMenuScene::update(sf::Time dt)
    {

    }

    void MainMenuScene::draw()
    {
        auto &sceneRender = _engine.sceneRender();
        sceneRender.texture().clear();

        _background->draw(sceneRender);
    }
} // space