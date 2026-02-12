#include "ModePremiere.h"

void ModePremiere::setup(lv_obj_t* parent) {
    lv_obj_t* label = lv_label_create(parent);
    lv_label_set_text(label, "Premiere Pro Mode Active");
    lv_obj_center(label);
}

void ModePremiere::loop() {
}

void ModePremiere::cleanup() {
}
