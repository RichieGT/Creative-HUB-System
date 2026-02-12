#include "Sidebar.h"
#include "NetworkManager.h"
#include "ModeAfterEffects.h"
#include "ModePremiere.h"
#include "ModeWorkLog.h"
#include "ModeDeck.h" 

// Define IDs for buttons
#define BTN_ID_DECK 99
#define BTN_ID_AE 0
#define BTN_ID_PREMIERE 1
#define BTN_ID_WORKLOG 2
#define BTN_ID_XIAOZHI 3

lv_obj_t* Sidebar::wifiLabel = nullptr;

void Sidebar::init(lv_obj_t* parent) {
    // Create a container for the sidebar on the left
    lv_obj_t* cont = lv_obj_create(parent);
    lv_obj_set_size(cont, 200, lv_pct(100)); // 200px width, 100% height
    lv_obj_align(cont, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_pad_all(cont, 10, 0);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // UI Styles (reuse container style)
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x151515), LV_PART_MAIN); // Slightly darker
    lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);

    // WiFi Status Label (Top)
    wifiLabel = lv_label_create(cont);
    lv_label_set_text(wifiLabel, "WiFi: ...");
    lv_obj_set_style_text_color(wifiLabel, lv_color_hex(0x888888), 0);
    
    // Add Buttons
    create_button(cont, "DECK", BTN_ID_DECK); // Home/Deck
    create_button(cont, "XIAOZHI", BTN_ID_XIAOZHI); // New Button
    create_button(cont, "After Effects", BTN_ID_AE);
    create_button(cont, "Premiere", BTN_ID_PREMIERE);
    create_button(cont, "Work Log", BTN_ID_WORKLOG);

    // Create a timer to update status
    lv_timer_create(update_status, 2000, NULL);
}

void Sidebar::update_status(lv_timer_t* t) {
    if (wifiLabel) {
        if (NetworkManager::isConnected()) {
            lv_label_set_text(wifiLabel, "WiFi: OK");
            lv_obj_set_style_text_color(wifiLabel, lv_color_hex(0x00FF00), 0);
        } else {
            lv_label_set_text(wifiLabel, "WiFi: --");
            lv_obj_set_style_text_color(wifiLabel, lv_color_hex(0xFF0000), 0);
        }
    }
}

lv_obj_t* Sidebar::create_button(lv_obj_t* parent, const char* label, int id) {
    lv_obj_t* btn = lv_button_create(parent);
    lv_obj_set_width(btn, lv_pct(100));
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, (void*)(intptr_t)id);

    lv_obj_t* lbl = lv_label_create(btn);
    lv_label_set_text(lbl, label);
    lv_obj_center(lbl);

    return btn;
}

void Sidebar::event_handler(lv_event_t* e) {
    int id = (int)(intptr_t)lv_event_get_user_data(e);

    switch(id) {
        case BTN_ID_AE:
            WindowManager::getInstance().switchTo(new ModeAfterEffects());
            break;
        case BTN_ID_PREMIERE:
            WindowManager::getInstance().switchTo(new ModePremiere());
            break;
        case BTN_ID_WORKLOG:
            WindowManager::getInstance().switchTo(new ModeWorkLog());
            break;
        case BTN_ID_DECK:
            WindowManager::getInstance().switchTo(new ModeDeck());
            break;
        case BTN_ID_XIAOZHI:
             // Send Wake Command or similar
             NetworkManager::sendCerebroCommand("wake_xiaozhi");
             break;
    }
}
