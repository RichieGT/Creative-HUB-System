#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WebSocketsClient.h>

class NetworkManager {
public:
    static void init(const char* ssid, const char* password);
    static void loop();
    
    static void sendWebhook(const char* action);
    
    // Enviar comando al Mac/cerebro vía WebSocket
    static void sendCerebroCommand(const char* command, const char* args = nullptr);
    
    static void connectWebSocket(const char* host, int port);
    static bool isConnected();

private:
    static void webhookTask(void* parameter);
    static void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);
    
    static QueueHandle_t webhookQueue;
    static WebSocketsClient webSocket;
    static String macStatus;
};