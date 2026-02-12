#include "ModeAfterEffects.h"
#include "EventSystem.h"
#include "HIDManager.h"
#include "UI_Styles.h"

// Button IDs
#define BTN_EASY_EASE 0
#define BTN_SPLIT_LAYER 1
#define BTN_NULL_OBJ 2
#define BTN_NEW_COMP 3
#define BTN_RENDER 4
#define BTN_TRIM 5
#define SLIDER_TIMELINE 100

void ModeAfterEffects::setup(lv_obj_t* parent) {
    // Initialize Styles if not already done
    UI_Styles::init();
    
    // Apply Background Style to Parent/Content Area
    UI_Styles::applyContainerStyle(parent);

    // 1. Create a Grid Container for Buttons (Take top 80% height)
    static int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_t* grid = lv_obj_create(parent);
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
    lv_obj_set_size(grid, lv_pct(100), lv_pct(80)); // 80% height
    lv_obj_align(grid, LV_ALIGN_TOP_MID, 0, 0);
    
    UI_Styles::applyContainerStyle(grid); // Transparent/Dark BG
    lv_obj_set_style_pad_all(grid, 20, 0);
    lv_obj_set_style_pad_row(grid, 15, 0);
    lv_obj_set_style_pad_column(grid, 15, 0);
    
    // 2. Create Buttons
    createButton(grid, "Easy Ease\nF9", BTN_EASY_EASE, 0, 0);
    createButton(grid, "Split Layer\nCmd+Shft+D", BTN_SPLIT_LAYER, 0, 1);
    
    createButton(grid, "Null Object\nCmd+Opt+Y", BTN_NULL_OBJ, 1, 0);
    createButton(grid, "New Comp\nCmd+N", BTN_NEW_COMP, 1, 1);
    
    createButton(grid, "Add Render\nCmd+Ctrl+M", BTN_RENDER, 2, 0);
    createButton(grid, "Trim Comp\nCmd+Shft+X", BTN_TRIM, 2, 1);

    // 3. Create Timeline Slider (Bottom 20%)
    lv_obj_t* slider = lv_slider_create(parent);
    UI_Styles::applySliderStyle(slider);
    lv_obj_set_size(slider, lv_pct(80), 30); // 80% width, 30px height (track)
    lv_obj_align(slider, LV_ALIGN_BOTTOM_MID, 0, -30); // 30px from bottom implies some margin
    lv_slider_set_range(slider, 0, 100);
    lv_slider_set_value(slider, 50, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider, event_handler, LV_EVENT_VALUE_CHANGED, (void*)(intptr_t)SLIDER_TIMELINE);

    // Label for Slider "Timeline Scrub"
    lv_obj_t* lbl = lv_label_create(parent);
    UI_Styles::applyLabelStyle(lbl);
    lv_label_set_text(lbl, "Timeline Scrub");
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, 0); // Smaller font for label
    lv_obj_align_to(lbl, slider, LV_ALIGN_OUT_TOP_MID, 0, -10);
}

void ModeAfterEffects::createButton(lv_obj_t* parent, const char* label, int id, int row, int col) {
    lv_obj_t* btn = lv_button_create(parent);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, (void*)(intptr_t)id);
    
    UI_Styles::applyButtonStyle(btn);

    lv_obj_t* lbl = lv_label_create(btn);
    UI_Styles::applyLabelStyle(lbl); // Sets large font and color
    lv_label_set_text(lbl, label);
    lv_obj_center(lbl);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, 0);
}

void ModeAfterEffects::event_handler(lv_event_t* e) {
    int id = (int)(intptr_t)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    
    if (code == LV_EVENT_VALUE_CHANGED && id == SLIDER_TIMELINE) {
        // Handle Slider Logic (e.g., send scroll commands or specialized HID)
        // For now just log
        // int val = lv_slider_get_value(lv_event_get_target(e));
        // Serial.println(val);
        return;
    }

    if (code == LV_EVENT_CLICKED) {
         EventSystem::sendCommand("hid_action", String(id).c_str());

        switch(id) {
            case BTN_EASY_EASE:
                HIDManager::sendMacCommand(KEY_F9); 
                break;

            case BTN_SPLIT_LAYER:
                HIDManager::sendMacCommand('d', HIDManager::KEY_MOD_CMD | HIDManager::KEY_MOD_SHIFT);
                break;

            case BTN_NULL_OBJ:
                HIDManager::sendMacCommand('y', HIDManager::KEY_MOD_CMD | HIDManager::KEY_MOD_OPT);
                break;

            case BTN_NEW_COMP:
                HIDManager::sendMacCommand('n', HIDManager::KEY_MOD_CMD);
                break;
                
            case BTN_RENDER:
                HIDManager::sendMacCommand('m', HIDManager::KEY_MOD_CMD | HIDManager::KEY_MOD_CTRL);
                break;

            case BTN_TRIM:
                HIDManager::sendMacCommand('x', HIDManager::KEY_MOD_CMD | HIDManager::KEY_MOD_SHIFT);
                break;
        }
    }
}

void ModeAfterEffects::loop() {
    // No specific loop logic needed for now
}

void ModeAfterEffects::cleanup() {
    // Grid and buttons are automatically cleaned up when 'parent' is cleared
}
