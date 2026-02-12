#pragma once

#include <Arduino.h>
#include <USB.h>
#include <USBHIDKeyboard.h>

class HIDManager {
public:
    static void init();
    static void sendMacCommand(uint16_t key, uint8_t modifiers = 0);
    static void sendString(const char* str);
    
    // Mac Modifiers
    static const uint8_t KEY_MOD_CMD = KEY_LEFT_GUI;
    static const uint8_t KEY_MOD_SHIFT = KEY_LEFT_SHIFT;
    static const uint8_t KEY_MOD_OPT = KEY_LEFT_ALT;
    static const uint8_t KEY_MOD_CTRL = KEY_LEFT_CTRL;

private:
    static USBHIDKeyboard Keyboard;
};
