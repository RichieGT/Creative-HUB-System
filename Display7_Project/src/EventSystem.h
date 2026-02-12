#pragma once

#include <Arduino.h>

namespace EventSystem {

    /**
     * @brief Sends a formatted JSON command to the Serial port.
     * 
     * @param action The action to perform (e.g., "change_mode", "keyboard_shortcut")
     * @param target The target or parameter for the action (e.g., "AE", "CTRL+Z")
     */
    inline void sendCommand(const char* action, const char* target) {
        Serial.print("{\"action\": \"");
        Serial.print(action);
        Serial.print("\", \"target\": \"");
        Serial.print(target);
        Serial.println("\"}");
    }

}
