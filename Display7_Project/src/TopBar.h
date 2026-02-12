#pragma once

#include <lvgl.h>

class TopBar {
public:
    static void init(lv_obj_t* parent);
    static void loop(); 

private:
   static void update_timer(lv_timer_t* t);
   
   static lv_obj_t* statusLabel;
   static lv_obj_t* vuBar;
   static lv_obj_t* wifiIcon;
};
