#include "ModeWorkLog.h"
#include "UI_Styles.h"
#include "NetworkManager.h"
#include "EventSystem.h"

#define BTN_START_TASK 0
#define BTN_RENDER_PAUSE 1
#define BTN_END_TASK 2

void ModeWorkLog::setup(lv_obj_t* parent) {
    UI_Styles::init();
    UI_Styles::applyContainerStyle(parent);

    // Vertical layout
    lv_obj_t* cont = lv_obj_create(parent);
    lv_obj_set_size(cont, lv_pct(80), lv_pct(100)); // 80% width centered
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(cont, 30, 0); // Gap between buttons
    UI_Styles::applyContainerStyle(cont); // Transparent

    // Create 3 Large Buttons
    createButton(cont, "INICIO TAREA\n(Start)", BTN_START_TASK, lv_color_hex(0x28A745)); // Green
    createButton(cont, "PAUSA / RENDER", BTN_RENDER_PAUSE, lv_color_hex(0xFFC107)); // Yellow
    createButton(cont, "FINALIZAR\n(End)", BTN_END_TASK, lv_color_hex(0xDC3545)); // Red
}

void ModeWorkLog::createButton(lv_obj_t* parent, const char* label, int id, lv_color_t color) {
    lv_obj_t* btn = lv_button_create(parent);
    lv_obj_set_size(btn, lv_pct(100), 100); // Full width of container, 100px height
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, (void*)(intptr_t)id);
    
    UI_Styles::applyButtonStyle(btn); // Base Styles
    lv_obj_set_style_bg_color(btn, color, LV_PART_MAIN); // Override color

    lv_obj_t* lbl = lv_label_create(btn);
    UI_Styles::applyLabelStyle(lbl);
    lv_label_set_text(lbl, label);
    lv_obj_center(lbl);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, 0);
}

void ModeWorkLog::event_handler(lv_event_t* e) {
    int id = (int)(intptr_t)lv_event_get_user_data(e);
    
    // UI Feedback (Simple Toast or Log)
    EventSystem::sendCommand("worklog_action", String(id).c_str());

    switch(id) {
        case BTN_START_TASK:
            NetworkManager::sendWebhook("task_start");
            break;
        case BTN_RENDER_PAUSE:
            NetworkManager::sendWebhook("render_start");
            break;
        case BTN_END_TASK:
            NetworkManager::sendWebhook("task_end");
            break;
    }
}

void ModeWorkLog::loop() {}
void ModeWorkLog::cleanup() {}
