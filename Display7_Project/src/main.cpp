#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include <TAMC_GT911.h>
#include <Wire.h> 

#include "WindowManager.h"
#include "Sidebar.h"
#include "EventSystem.h"
#include "HIDManager.h"
#include "NetworkManager.h"
#include "BottomBar.h"
#include "GT911_Config.h" // Include the config header
void configureGT911(); // Forward declaration if not in header


// =============================================================================
// HARDWARE CONFIGURATION
// =============================================================================

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    TFT_DE, TFT_VSYNC, TFT_HSYNC, TFT_PCLK,
    TFT_R0, TFT_R1, TFT_R2, TFT_R3, TFT_R4,
    TFT_G0, TFT_G1, TFT_G2, TFT_G3, TFT_G4, TFT_G5,
    TFT_B0, TFT_B1, TFT_B2, TFT_B3, TFT_B4,
    TFT_HSYNC_POLARITY, TFT_HSYNC_FRONT_PORCH, TFT_HSYNC_PULSE_WIDTH, TFT_HSYNC_BACK_PORCH,
    TFT_VSYNC_POLARITY, TFT_VSYNC_FRONT_PORCH, TFT_VSYNC_PULSE_WIDTH, TFT_VSYNC_BACK_PORCH,
    TFT_PCLK_ACTIVE_NEG, TFT_PREFER_SPEED
);

Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    TFT_WIDTH, TFT_HEIGHT, bus, 0 /* rotation */, true /* auto_flush */
);

TAMC_GT911 ts = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, TOUCH_WIDTH, TOUCH_HEIGHT);

// LVGL Buffers
#define DRAW_BUF_SIZE (800 * 480 * 2 / 10)
uint8_t* draw_buf1;
uint8_t* draw_buf2;

// =============================================================================
// LVGL CALLBACKS
// =============================================================================

void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);
    lv_display_flush_ready(disp);
}

// =============================================================================
// SAFE TOUCH DRIVER (Polling Mode)
// =============================================================================
// Avoiding interrupt callbacks preventing potential I2C crashes/resets.

volatile int16_t g_touchX = 0;
volatile int16_t g_touchY = 0;
volatile bool g_isTouched = false;

void pollGT911() {
    // Limit polling rate to ~30Hz to save CPU/Bus bandwidth
    static unsigned long lastPoll = 0;
    if (millis() - lastPoll < 30) return; 
    lastPoll = millis();

    uint8_t status = 0;
    Wire.beginTransmission(0x14);
    Wire.write(0x81); Wire.write(0x4E); // Read Status Register
    Wire.endTransmission();
    Wire.requestFrom(0x14, 1);
    if (Wire.available()) status = Wire.read();

    // Check if buffer bit (0x80) is set
    if (status & 0x80) {
        Wire.beginTransmission(0x14);
        Wire.write(0x81); Wire.write(0x50); // Read Coordinates
        Wire.endTransmission();
        
        Wire.requestFrom(0x14, 4);
        if (Wire.available() >= 4) {
             uint8_t xL = Wire.read();
             uint8_t xH = Wire.read();
             uint8_t yL = Wire.read();
             uint8_t yH = Wire.read();
             
             g_touchX = xL | (xH << 8);
             g_touchY = yL | (yH << 8);
             g_isTouched = true;
             
             // Log only valid touches
             Serial.printf("TOUCH DETECTED: X=%d Y=%d\n", g_touchX, g_touchY);
        }
        
        // CLEAR Status Register (Mandatory handshake)
        Wire.beginTransmission(0x14);
        Wire.write(0x81); Wire.write(0x4E); Wire.write(0x00);
        Wire.endTransmission();
    } else {
        g_isTouched = false;
    }
}

// Lightweight LVGL Callback (Reads from Globals)
void my_touch_read(lv_indev_t *indev, lv_indev_data_t *data) {
    if (g_isTouched) {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = g_touchX;
        data->point.y = g_touchY;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

// =============================================================================
// SETUP & LOOP
// =============================================================================

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Starting Creative Hub OS...");

    // 1. Init System Managers
     // HIDManager::init(); // DISABLED TEMPORARILY FOR STABILITY
    // NetworkManager::init("CreativeHub", "password"); // DISABLED here, called later
    // AudioManager::init(); // DISABLED (No Hardware)

    // 2. Hardware Enable via XL9535 (PROVEN SEQUENCE)
    Wire.begin(TOUCH_SDA, TOUCH_SCL);
    // Config Output Register (0x06): 0x00 = All Outputs
    Wire.beginTransmission(0x20); Wire.write(0x06); Wire.write(0x00); Wire.write(0x00); Wire.endTransmission();
    
    // --- RESET SEQUENCE for GT911 ---
    // P1.0 is likely RST, P1.1 or similar might be INT (or INT is direct). 
    // Waveshare ESP32-S3-Touch-LCD-7 uses XL9535.
    // Let's toggle lines to ensure reset.
    // 0x02 is Output Port 0, 0x03 is Output Port 1.
    // Let's Try: Drive Low -> Wait -> Drive High
    
    // Low
    Wire.beginTransmission(0x20); Wire.write(0x02); Wire.write(0x00); Wire.write(0x00); Wire.endTransmission();
    delay(100);
    // High (Enable)
    Wire.beginTransmission(0x20); Wire.write(0x02); Wire.write(0xFF); Wire.write(0xFF); Wire.endTransmission();
    delay(200);

    // 3. Init Display
    if (!gfx->begin()) Serial.println("GFX Failed!");
    gfx->fillScreen(BLACK); 
    
    // 4. Init Touch (PROVEN SEQUENCE with 0x14)
    ts.begin(0x14);
    ts.setRotation(ROTATION_NORMAL);
    
    configureGT911(); // ENABLED: Overwrite with correct 800x480 config
    Serial.println("Touch Initialized.");
    
    // --- VERIFY CONFIG (Diagnostic) ---
    Wire.beginTransmission(0x14);
    Wire.write(0x80); Wire.write(0x47);
    Wire.endTransmission();
    Wire.requestFrom(0x14, 5); // Read Version + X_H + X_L + Y_H + Y_L
    if (Wire.available() >= 5) {
        uint8_t ver = Wire.read();
        uint8_t xH = Wire.read();
        uint8_t xL = Wire.read();
        uint8_t yH = Wire.read();
        uint8_t yL = Wire.read();
        
        Serial.printf("READ CHECK: Version: 0x%02X\n", ver);
        Serial.printf("Resolution: X=%d Y=%d\n", (xH << 8) + xL, (yH << 8) + yL);
    }
    // ----------------------------------
    
    // --- READ PRODUCT ID (Diagnostic) ---
    Wire.beginTransmission(0x14);
    Wire.write(0x81); Wire.write(0x40);
    Wire.endTransmission();
    Wire.requestFrom(0x14, 4);
    if (Wire.available() >= 4) {
        char id[5];
        id[0] = Wire.read();
        id[1] = Wire.read();
        id[2] = Wire.read();
        id[3] = Wire.read();
        id[4] = 0;
        Serial.printf("GT911 Product ID: %s\n", id);
    } else {
        Serial.println("Failed to read Product ID!");
    }
    // ------------------------------------

    // 5. Init LVGL
    lv_init();

    draw_buf1 = (uint8_t*)heap_caps_malloc(DRAW_BUF_SIZE, MALLOC_CAP_SPIRAM);
    draw_buf2 = (uint8_t*)heap_caps_malloc(DRAW_BUF_SIZE, MALLOC_CAP_SPIRAM);

    if (!draw_buf1 || !draw_buf2) {
        Serial.println("Failed to allocate PSRAM buffers!");
        return;
    }

    lv_display_t *disp = lv_display_create(800, 480);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, draw_buf1, draw_buf2, DRAW_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touch_read);

    // --- DEBUG: VISUAL CURSOR FOR TOUCH CALIBRATION ---
    lv_obj_t *cursor_obj = lv_obj_create(lv_screen_active());
    lv_obj_set_size(cursor_obj, 20, 20);
    lv_obj_set_style_radius(cursor_obj, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(cursor_obj, lv_color_hex(0xFF0000), 0); // RED Cursor
    lv_obj_set_style_bg_opa(cursor_obj, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(cursor_obj, 2, 0);
    lv_obj_set_style_border_color(cursor_obj, lv_color_white(), 0);
    lv_indev_set_cursor(indev, cursor_obj); // Bind cursor to input
    // --------------------------------------------------

    // 6. UI Initialization
    lv_obj_t* scr = lv_screen_active();
    
    Sidebar::init(scr);
    BottomBar::init(scr);

    lv_obj_t* contentArea = lv_obj_create(scr);
    lv_obj_set_size(contentArea, 600, lv_pct(100));
    lv_obj_align(contentArea, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_pad_all(contentArea, 0, 0);
    
    WindowManager::getInstance().init(contentArea);

#include "ModeDeck.h" 

    // 7. Init Network (Last to avoid I2C interference during boot)
    Serial.println("Initializing Network...");
    NetworkManager::init("Familia DLO WRK", "Of1c1na123"); 
    
    // 8. AUTOLAUNCH DECK MODE
    Serial.println("Launching Deck Mode...");
    WindowManager::getInstance().switchTo(new ModeDeck());

    Serial.println("System Ready.");
}

void loop() {
    pollGT911(); // Call Safe Touch Poller
    lv_timer_handler();
    WindowManager::getInstance().update();
    NetworkManager::loop();
    
    static unsigned long last = 0;
    if (millis() - last > 1000) {
        last = millis();
        // Serial.println("Loop Alive...");
    }
    
    delay(5);
}
