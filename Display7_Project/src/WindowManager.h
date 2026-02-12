#pragma once

#include "App.h"
#include <lvgl.h>

class WindowManager {
public:
    static WindowManager& getInstance();

    /**
     * @brief Initialize the Window Manager.
     * @param contentArea The LVGL object that will act as the container for Apps.
     */
    void init(lv_obj_t* contentArea);

    /**
     * @brief Switch to a new Application/Mode.
     * Destroys the current app and initializes the new one.
     * @param newApp Pointer to the new App instance. pass 'new ModeClass()'
     */
    void switchTo(App* newApp);

    /**
     * @brief Update the current running app.
     * Should be called in the main loop.
     */
    void update();

private:
    WindowManager() : _currentApp(nullptr), _contentArea(nullptr) {} // Private constructor
    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;

    App* _currentApp;
    lv_obj_t* _contentArea;
};
