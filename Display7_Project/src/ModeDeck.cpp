#include "ModeDeck.h"
#include "UI_Styles.h"
#include "HIDManager.h"
#include "NetworkManager.h"

// =============================================================================
// DECK CONFIGURATION
// =============================================================================
// Type: 0=Macro (Teclado), 1=Webhook (Power Automate), 2=Xiaozhi (Mac Voice/Apps)
const DeckAction ModeDeck::actions[8] = {
    // FILA 1: XIAOZHI & SISTEMA
    { "XIAOZHI\nHOLA",    2, 0, "Hola Ricardo, Xiaozhi está listo para trabajar" }, 
    { "ABRIR\nAE",        2, 0, "abrir_ae" },             
    { "LOG\nTRABAJO",    1, 0, "registro_bitacora" }, // Power Automate Webhook             
    { "LUCES\n💡",        1, 0, "toggle_lights" },        

    // FILA 2: AFTER EFFECTS MACROS (Vía HID)
    { "Copy\nCmd+C",     0, 'c', (const char*)HIDManager::KEY_MOD_CMD }, 
    { "Paste\nCmd+V",    0, 'v', (const char*)HIDManager::KEY_MOD_CMD }, 
    { "Undo\nCmd+Z",     0, 'z', (const char*)HIDManager::KEY_MOD_CMD }, 
    { "Render\nQueue",   0, 'q', (const char*)(HIDManager::KEY_MOD_CMD | HIDManager::KEY_MOD_OPT) }
};

void ModeDeck::setup(lv_obj_t* parent) {
    UI_Styles::init();
    UI_Styles::applyContainerStyle(parent);

    static int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_t* grid = lv_obj_create(parent);
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
    lv_obj_set_size(grid, lv_pct(100), lv_pct(100));
    lv_obj_center(grid);
    
    UI_Styles::applyContainerStyle(grid);
    lv_obj_set_style_pad_all(grid, 20, 0); // Increase padding slightly
    lv_obj_set_style_pad_row(grid, 20, 0); // Gap between rows
    lv_obj_set_style_pad_column(grid, 20, 0); // Gap between columns

    int idx = 0;
    for(int r=0; r<ROWS; r++) {
        for(int c=0; c<COLS; c++) {
            if(idx < 8) {
                createButton(grid, idx, r, c);
                idx++;
            }
        }
    }
}

void ModeDeck::createButton(lv_obj_t* parent, int index, int row, int col) {
    lv_obj_t* btn = lv_button_create(parent);
    lv_obj_set_size(btn, 120, 120); // Fixed size
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_CENTER, col, 1, LV_GRID_ALIGN_CENTER, row, 1);
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, (void*)(intptr_t)index);
    UI_Styles::applyButtonStyle(btn);

    lv_obj_t* lbl = lv_label_create(btn);
    lv_obj_set_style_text_color(lbl, lv_color_white(), 0);
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, 0);
    lv_label_set_text(lbl, actions[index].label);
    lv_obj_center(lbl);
}

void ModeDeck::event_handler(lv_event_t* e) {
    int idx = (int)(intptr_t)lv_event_get_user_data(e);
    if(idx < 0 || idx >= 8) return;

    DeckAction action = actions[idx];
    Serial.printf("Ejecutando: %s\n", action.label);

    if (action.type == 0) { // MACRO HID
        uint8_t mods = 0;
        if (action.payload != "") mods = (uint8_t)(intptr_t)action.payload; 
        HIDManager::sendMacCommand(action.data, mods);
    } 
    else if (action.type == 1) { // WEBHOOK (Power Automate)
        NetworkManager::sendWebhook(action.payload);
    }
    else if (action.type == 2) { // XIAOZHI (Brain Mac)
        // Si el payload contiene "abrir_ae", enviamos comando de apertura
        if (String(action.payload) == "abrir_ae") {
            NetworkManager::sendCerebroCommand("abrir_ae", "");
        } else {
            // De lo contrario, usamos el payload como texto para que el Mac hable
            NetworkManager::sendCerebroCommand("hablar", action.payload);
        }
    }
}

void ModeDeck::loop() { }
void ModeDeck::cleanup() { }