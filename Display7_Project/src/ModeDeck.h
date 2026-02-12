#pragma once

#include "App.h"
#include <lvgl.h>
#include <Arduino.h>

struct DeckAction {
    const char* label;   // Texto o Icono del botón
    uint8_t type;        // 0: Macro, 1: Webhook (Power Automate), 2: Xiaozhi (Mac)
    uint32_t data;       // Código de tecla (para Macros)
    const char* payload; // Payload para Webhook o Texto para que Xiaozhi hable
};

class ModeDeck : public App {
public:
    // Funciones obligatorias de la clase App
    void setup(lv_obj_t* parent) override;
    void loop() override;
    void cleanup() override;

private:
    // Manejador de clics en los botones
    static void event_handler(lv_event_t* e);
    
    // Función auxiliar para dibujar cada botón en la cuadrícula
    void createButton(lv_obj_t* parent, int index, int row, int col);
    
    // Configuración de la rejilla (2 filas x 4 columnas = 8 botones)
    static const int ROWS = 2;
    static const int COLS = 4;
    static const DeckAction actions[8];
};