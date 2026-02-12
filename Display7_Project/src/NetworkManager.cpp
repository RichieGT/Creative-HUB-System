#include "NetworkManager.h"
#include "WindowManager.h"
#include "ModeAfterEffects.h"

// REPLACE WITH REAL CREDENTIALS
const char* _ssid = "Familia DLO WRK";
const char* _password = "Of1c1na123";
const char* _webhookUrl = "YOUR_POWER_AUTOMATE_URL"; 
const char* _wsHost = "192.168.1.170"; // Placeholder Mac IP
int _wsPort = 5050; // WebSocket port (brain.py escucha aquí)

QueueHandle_t NetworkManager::webhookQueue;
WebSocketsClient NetworkManager::webSocket;
String NetworkManager::macStatus = "Disconnected";

void NetworkManager::init(const char* ssid, const char* password) {
    _ssid = ssid;
    _password = password;

    webhookQueue = xQueueCreate(10, sizeof(const char*));

    xTaskCreatePinnedToCore(webhookTask, "WebhookTask", 8192, NULL, 1, NULL, 0);

    WiFi.begin(_ssid, _password);
    
    // We need to wait for WiFi before connecting WS, usually handled in loop or event
    // For simplicity, we configure WS here but it won't connect until WiFi is ready
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);
}

void NetworkManager::connectWebSocket(const char* host, int port) {
    _wsHost = host;
    _wsPort = port;
    webSocket.begin(_wsHost, _wsPort, "/");
}

void NetworkManager::loop() {
    // Must be called freq in main loop
    webSocket.loop();
}

bool NetworkManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void NetworkManager::sendWebhook(const char* action) {
    if (xQueueSend(webhookQueue, &action, 0) != pdTRUE) {
        Serial.println("Queue Full");
    }
}

void NetworkManager::webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.println("[WS] Disconnected!");
            macStatus = "Disconnected";
            break;
        case WStype_CONNECTED:
            Serial.printf("[WS] Connected to: %s\n", payload);
            macStatus = "Connected";
            webSocket.sendTXT("{\"device\": \"CreativeHub\"}");
            break;
        case WStype_TEXT:
            Serial.printf("[WS] Text: %s\n", payload);
            
            // Parse JSON Command from Mac
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, payload);
            if (!error) {
                if (doc["cmd"].is<const char*>()) {
                    const char* cmd = doc["cmd"];
                    if (strcmp(cmd, "switch_mode") == 0) {
                        const char* target = doc["args"];
                        // Helper to switch modes strings
                        if (strcmp(target, "AE") == 0) WindowManager::getInstance().switchTo(new ModeAfterEffects());
                        // Add others as needed
                    }
                }
                if (doc["status"].is<const char*>()) {
                    macStatus = doc["status"].as<String>();
                }
            }
            break;
    }
}

void NetworkManager::webhookTask(void* parameter) {
    const char* action;
    while (true) {
        if (xQueueReceive(webhookQueue, &action, portMAX_DELAY)) {
             if (NetworkManager::isConnected()) {
                HTTPClient http;
                http.begin(_webhookUrl);
                http.addHeader("Content-Type", "application/json");
                JsonDocument doc;
                doc["user"] = "Ricardo";
                doc["action"] = action;
                doc["timestamp"] = millis();
                String rb;
                serializeJson(doc, rb);
                http.POST(rb);
                http.end();
             }
        }
    }
}

void NetworkManager::sendCerebroCommand(const char* command, const char* args) {
    if (macStatus == "Connected") {
        JsonDocument doc;
        doc["cmd"] = "cerebro";
        doc["action"] = command;
        if (args) doc["args"] = args;
        String out;
        serializeJson(doc, out);
        webSocket.sendTXT(out);
    } else {
        Serial.printf("[Cerebro] Not connected, cannot send: %s\n", command);
    }
}

