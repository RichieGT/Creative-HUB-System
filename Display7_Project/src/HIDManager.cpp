#include "HIDManager.h"

USBHIDKeyboard HIDManager::Keyboard;

void HIDManager::init() {
    USB.productName("Creative Hub Controller");
    USB.manufacturerName("CreativeHub");
    USB.begin(); // Must be called before Keyboard.begin() for some core versions, or just to ensure USB stack logic
    Keyboard.begin();
}

void HIDManager::sendMacCommand(uint16_t key, uint8_t modifiers) {
    if (modifiers != 0) {
        Keyboard.press(modifiers);
    }
    Keyboard.press(key);
    delay(10); // Short delay to ensure registration
    Keyboard.releaseAll();
}

void HIDManager::sendString(const char* str) {
    Keyboard.print(str);
}
