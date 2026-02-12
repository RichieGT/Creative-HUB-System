#pragma once

#include "App.h"
#include <lvgl.h>

class ModePremiere : public App {
public:
    void setup(lv_obj_t* parent) override;
    void loop() override;
    void cleanup() override;
};
