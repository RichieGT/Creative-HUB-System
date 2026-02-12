#pragma once

#include <lvgl.h>

/**
 * @brief Base class for all Modes/Apps in the system.
 */
class App {
public:
    virtual ~App() {}

    /**
     * @brief Called when the app is started.
     * Initialize UI elements here.
     * @param parent The parent object (usually the content area container).
     */
    virtual void setup(lv_obj_t* parent) = 0;

    /**
     * @brief Called periodically in the main loop.
     * Handle logic, animations, or sensor updates here.
     */
    virtual void loop() {}

    /**
     * @brief Called before the app is destroyed/switched away.
     * Clean up resources, stop timers, etc.
     * Note: UI elements created on 'parent' in setup() are usually cleaned up automatically
     * if the parent is cleared or deleted, but specific app logic cleanup goes here.
     */
    virtual void cleanup() {}
};
