#pragma once

#include <lvgl.h>

class UI_Styles {
public:
    static void init();
    
    // Application Styles
    static void applyButtonStyle(lv_obj_t* btn);
    static void applyLabelStyle(lv_obj_t* label);
    static void applySliderStyle(lv_obj_t* slider);
    static void applyContainerStyle(lv_obj_t* cont);

    // Colors
    static lv_color_t COLOR_BG;      // #1E1E1E
    static lv_color_t COLOR_BTN;     // #323232
    static lv_color_t COLOR_BTN_PR;  // #454545 (Pressed)
    static lv_color_t COLOR_ACCENT;  // #0078D7 (Blue)
    static lv_color_t COLOR_TEXT;    // #FFFFFF
    
private:
    static lv_style_t style_btn;
    static lv_style_t style_btn_pr;
    static lv_style_t style_label;
    static lv_style_t style_slider_main;
    static lv_style_t style_slider_indic;
    static lv_style_t style_slider_knob;
};
