#pragma once

#include <lvgl.h>

class BottomBar {
public:
    static void init(lv_obj_t* parent);

private:
   static void update_timer(lv_timer_t* t);
   
   static lv_obj_t* statusLabel;
   static lv_obj_t* vuBar;
   static lv_obj_t* container;
};
