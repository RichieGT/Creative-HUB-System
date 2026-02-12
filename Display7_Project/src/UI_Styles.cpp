#include "UI_Styles.h"

lv_color_t UI_Styles::COLOR_BG = lv_color_hex(0x1E1E1E);
lv_color_t UI_Styles::COLOR_BTN = lv_color_hex(0x323232);
lv_color_t UI_Styles::COLOR_BTN_PR = lv_color_hex(0x454545);
lv_color_t UI_Styles::COLOR_ACCENT = lv_color_hex(0x0078D7);
lv_color_t UI_Styles::COLOR_TEXT = lv_color_hex(0xFFFFFF);

lv_style_t UI_Styles::style_btn;
lv_style_t UI_Styles::style_btn_pr;
lv_style_t UI_Styles::style_label;
lv_style_t UI_Styles::style_slider_main;
lv_style_t UI_Styles::style_slider_indic;
lv_style_t UI_Styles::style_slider_knob;

void UI_Styles::init() {
    // Button Default Style
    lv_style_init(&style_btn);
    lv_style_set_bg_color(&style_btn, COLOR_BTN);
    lv_style_set_bg_opa(&style_btn, LV_OPA_COVER);
    lv_style_set_border_width(&style_btn, 0);
    lv_style_set_radius(&style_btn, 8);
    lv_style_set_shadow_width(&style_btn, 0); // Flat look typical of Adobe
    lv_style_set_text_color(&style_btn, COLOR_TEXT);

    // Button Pressed Style
    lv_style_init(&style_btn_pr);
    lv_style_set_bg_color(&style_btn_pr, COLOR_BTN_PR);
    lv_style_set_transform_width(&style_btn_pr, -2); // Slight shrink
    lv_style_set_transform_height(&style_btn_pr, -2);

    // Label Style (Large Font)
    lv_style_init(&style_label);
    lv_style_set_text_color(&style_label, COLOR_TEXT);
    // Ensure LV_FONT_MONTSERRAT_24 is enabled in lv_conf.h
    // lv_style_set_text_font(&style_label, &lv_font_montserrat_24); // Use default font 

    // Slider Styles
    lv_style_init(&style_slider_main); // Track
    lv_style_set_bg_color(&style_slider_main, lv_color_hex(0x151515)); // Darker slot
    lv_style_set_radius(&style_slider_main, 4);

    lv_style_init(&style_slider_indic); // Filled part
    lv_style_set_bg_color(&style_slider_indic, COLOR_ACCENT);
    lv_style_set_radius(&style_slider_indic, 4);

    lv_style_init(&style_slider_knob); // Knob
    lv_style_set_bg_color(&style_slider_knob, COLOR_TEXT);
    lv_style_set_bg_opa(&style_slider_knob, LV_OPA_COVER);
    lv_style_set_border_width(&style_slider_knob, 2);
    lv_style_set_border_color(&style_slider_knob, COLOR_ACCENT);
    lv_style_set_pad_all(&style_slider_knob, 8); // Make knob larger
}

void UI_Styles::applyButtonStyle(lv_obj_t* btn) {
    lv_obj_add_style(btn, &style_btn, LV_PART_MAIN);
    lv_obj_add_style(btn, &style_btn_pr, LV_STATE_PRESSED);
}

void UI_Styles::applyLabelStyle(lv_obj_t* label) {
    lv_obj_add_style(label, &style_label, LV_PART_MAIN);
}

void UI_Styles::applySliderStyle(lv_obj_t* slider) {
    lv_obj_add_style(slider, &style_slider_main, LV_PART_MAIN);
    lv_obj_add_style(slider, &style_slider_indic, LV_PART_INDICATOR);
    lv_obj_add_style(slider, &style_slider_knob, LV_PART_KNOB);
}

void UI_Styles::applyContainerStyle(lv_obj_t* cont) {
    lv_obj_set_style_bg_color(cont, COLOR_BG, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);
}
