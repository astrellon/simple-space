#include "ui_manager.hpp"

#include "ui_window.hpp"
#include "ui_debug.hpp"
#include "ui_definitions.hpp"
#include "ui_objects.hpp"
#include "ui_frame_buffers.hpp"
#include "ui_photo_album.hpp"
#include "ui_compendium.hpp"
#include "ui_element_window.hpp"

#include "editor/ui_area_selector.hpp"
#include "editor/ui_selected.hpp"
#include "editor/ui_editor_menu.hpp"
#include "editor/ui_object_create.hpp"

namespace space
{
    UIManager::UIManager() : _uiSelected(nullptr), _uiObjects(nullptr), _uiFrameBuffers(nullptr),
        _uiPhotoAlbum(nullptr), _uiCompendium(nullptr), _uiElementWindow(nullptr), _uiAreaSelector(nullptr),
        _uiEditorMenu(nullptr), _uiObjectCreate(nullptr)
    {

    }

    void UIManager::initDefaultWindows()
    {
        _uiDebug = createWindow<UIDebug>();
        _uiDefinitions = createWindow<UIDefinitions>();
        _uiSelected = createWindow<UISelected>();
        _uiObjects = createWindow<UIObjects>();
        _uiFrameBuffers = createWindow<UIFrameBuffers>();
        _uiPhotoAlbum = createWindow<UIPhotoAlbum>();
        _uiCompendium = createWindow<UICompendium>();
        _uiElementWindow = createWindow<UIElementWindow>();
        _uiAreaSelector = createWindow<UIAreaSelector>();
        _uiEditorMenu = createWindow<UIEditorMenu>();
        _uiObjectCreate = createWindow<UIObjectCreate>();
    }

    void UIManager::draw(Engine &engine)
    {
        for (auto &window : _uiWindows)
        {
            window->draw(engine);
        }
    }
} // namespace space
