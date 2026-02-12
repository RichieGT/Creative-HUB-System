#include "TopBar.h"
#include "UI_Styles.h"
#include "NetworkManager.h"

lv_obj_t* TopBar::statusLabel;
// lv_obj_t* TopBar::vuBar; // Removed
lv_obj_t* TopBar::wifiIcon;

void TopBar::init(lv_obj_t* parent) {
    // Created as a container at the TOP of the passed parent (screen)
    lv_obj_t* cont = lv_obj_create(parent);
    lv_obj_set_size(cont, lv_pct(100), 40); // 40px height
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 0);
    
    // Style
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x252525), 0);
    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_style_radius(cont, 0, 0);
    lv_obj_set_style_pad_all(cont, 5, 0);

    // Flex layout
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // 1. Status Label (Left)
    statusLabel = lv_label_create(cont);
    lv_label_set_text(statusLabel, "Creative Hub");
    lv_obj_set_style_text_color(statusLabel, lv_color_hex(0xAAAAAA), 0);

    // 2. WiFi Icon (Right)
    wifiIcon = lv_label_create(cont);
    lv_label_set_text(wifiIcon, LV_SYMBOL_WIFI);
    lv_obj_set_style_text_color(wifiIcon, lv_color_hex(0x555555), 0);

    // Timer for updates
    lv_timer_create(update_timer, 500, NULL); // Slower update
}

void TopBar::update_timer(lv_timer_t* t) {
    // Update WiFi Icon
    if (NetworkManager::isConnected()) {
        lv_obj_set_style_text_color(wifiIcon, lv_color_hex(0x0078D7), 0);
    } else {
         lv_obj_set_style_text_color(wifiIcon, lv_color_hex(0x555555), 0);
    }
}
