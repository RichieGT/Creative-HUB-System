#pragma once

#include "App.h"
#include <lvgl.h>

class ModeAfterEffects : public App {
public:
    void setup(lv_obj_t* parent) override;
    void loop() override;
    void cleanup() override;

private:
    static void event_handler(lv_event_t* e);
    
    // Helper to create styled buttons
    void createButton(lv_obj_t* parent, const char* label, int id, int row, int col);
};
