#pragma once

#include <lvgl.h>
#include "WindowManager.h"
#include "ModeAfterEffects.h"
#include "ModePremiere.h"
#include "ModeWorkLog.h"
#include "ModeDeck.h"

class Sidebar {
public:
    static void init(lv_obj_t* parent);

private:
    static void event_handler(lv_event_t* e);
    static lv_obj_t* create_button(lv_obj_t* parent, const char* label, int id);
    
    // Status Logic
    static void update_status(lv_timer_t* t);
    static lv_obj_t* wifiLabel;
};
