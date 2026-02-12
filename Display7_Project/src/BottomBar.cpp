#include "BottomBar.h"
#include "UI_Styles.h"
#include "NetworkManager.h"

lv_obj_t* BottomBar::statusLabel;
// lv_obj_t* BottomBar::vuBar; // Removed
lv_obj_t* BottomBar::container;

void BottomBar::init(lv_obj_t* parent) {
    // Container at BOTTOM
    container = lv_obj_create(parent);
    lv_obj_set_size(container, lv_pct(100), 40); // 40px height
    lv_obj_align(container, LV_ALIGN_BOTTOM_MID, 0, 0);
    
    // Transparent dark style
    lv_obj_set_style_bg_color(container, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(container, LV_OPA_70, 0); 
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_radius(container, 0, 0);
    lv_obj_set_style_pad_all(container, 0, 0); // No padding for full width bar

    // Status Overlay
    statusLabel = lv_label_create(container);
    lv_label_set_text(statusLabel, "System Ready");
    lv_obj_set_style_text_color(statusLabel, lv_color_hex(0xFFFFFF), 0);
    lv_obj_center(statusLabel);
}

void BottomBar::update_timer(lv_timer_t* t) {
    // Placeholder for future status updates
}
