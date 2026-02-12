#include "WindowManager.h"

WindowManager& WindowManager::getInstance() {
    static WindowManager instance;
    return instance;
}

void WindowManager::init(lv_obj_t* contentArea) {
    _contentArea = contentArea;
}

void WindowManager::switchTo(App* newApp) {
    if (_currentApp) {
        _currentApp->cleanup();
        delete _currentApp;
        _currentApp = nullptr;
    }

    if (_contentArea) {
        // Clean all children of the content area to ensure a blank slate for the new app
        lv_obj_clean(_contentArea);
    }

    _currentApp = newApp;
    
    if (_currentApp && _contentArea) {
        _currentApp->setup(_contentArea);
    }
}

void WindowManager::update() {
    if (_currentApp) {
        _currentApp->loop();
    }
}
