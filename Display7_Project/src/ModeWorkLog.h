#pragma once

#include "App.h"
#include <lvgl.h>

class ModeWorkLog : public App {
public:
    void setup(lv_obj_t* parent) override;
    void loop() override;
    void cleanup() override;

private:
   static void event_handler(lv_event_t* e);
    // Helper
   void createButton(lv_obj_t* parent, const char* label, int id, lv_color_t color);
};
