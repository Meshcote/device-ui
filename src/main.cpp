#include "graphics/ILI9225Display.h"
#include "input/I2CKeyboardInputDriver.h"
#include "input/I2CKeyboardScanner.h"
#include "panel_manager.h"
#include "screen_manager.h"
#include <Arduino.h>
#include <Wire.h>

ILI9225Display display(7, 5, 6, 4, 3, 2);
I2CKeyboardInputDriver *g_keyboard = nullptr;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n\n╔═══════════════════════════════════════╗");
    Serial.println("║     MESHTASTIC DEVICE-UI v2.0        ║");
    Serial.println("║   Heltec WiFi LoRa V3 + ILI9225      ║");
    Serial.println("╚═══════════════════════════════════════╝\n");

    Serial.printf("Chip: %s\n", ESP.getChipModel());
    Serial.printf("CPU: %d MHz\n", ESP.getCpuFreqMHz());
    Serial.printf("Flash: %d KB\n", ESP.getFlashChipSize() / 1024);
    Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("PSRAM: %d bytes\n\n", ESP.getPsramSize());

    Serial.println("[INIT] Inicializando display ILI9225...");
    if (!display.init()) {
        Serial.println("[ERROR] No se pudo inicializar el display!");
        while (1)
            delay(1000);
    }

    Serial.println("[OK] Display inicializado");
    Serial.printf("[INFO] Resolución: %dx%d\n", display.getWidth(), display.getHeight());

    Serial.println("[INIT] Inicializando I2C...");
    Wire.begin(41, 42);
    Wire.setClock(100000);

    // Escanear BUS I2C completo para debug
    Serial.println("[I2C] Escaneando bus I2C completo...");
    int deviceCount = 0;
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            Serial.printf("[I2C] Dispositivo encontrado en 0x%02X\n", addr);
            deviceCount++;
        }
    }
    Serial.printf("[I2C] Total dispositivos encontrados: %d\n", deviceCount);
    Serial.println("[INIT] Escaneando CardKB (0x5F)...");
    I2CKeyboardScanner scanner;
    I2CKeyboardInputDriver *kbd = scanner.scan();

    if (kbd) {
        Serial.println("[OK] CardKB detectado!");
        Serial.println("[CardKB] Inicializando driver...");
        kbd->init();
        g_keyboard = kbd; // Guardar referencia global
        Serial.println("[CardKB] Driver inicializado - listo para recibir teclas");
    } else {
        Serial.println("[WARN] CardKB NO detectado");
    }

    Serial.println("\n[UI] Inicializando Device-UI...");
    ScreenManager::init();

    Serial.printf("\n=== Sistema listo === (Heap: %d bytes)\n\n", ESP.getFreeHeap());
}

void loop()
{
    static bool main_loaded = false;
    static unsigned long boot_time = millis();

    if (!main_loaded && (millis() - boot_time > 3000)) {
        Serial.println("[MAIN] Transición a pantalla principal...");
        ScreenManager::loadScreen(SCREEN_MAIN);
        main_loaded = true;
    }

    ScreenManager::tick();

    // Leer CardKB manualmente y simular input LVGL
    static unsigned long lastKbRead = 0;
    if (millis() - lastKbRead > 50) { // Cada 50ms
        lastKbRead = millis();

        Wire.requestFrom((uint8_t)0x5F, (uint8_t)1);
        if (Wire.available()) {
            char key = Wire.read();
            if (key != 0x00 && key != 0xE0) {
                Serial.printf("[CardKB] Tecla: 0x%02X\n", key);

                // Obtener grupo LVGL
                lv_group_t *grp = InputDriver::getInputGroup();
                if (grp) {
                    // Simular navegación manual
                    if (key == 0xB6) { // DOWN
                        lv_group_focus_next(grp);
                        lv_obj_t *focused = lv_group_get_focused(grp);
                        lv_obj_invalidate(focused); // Marcar como "dirty" para redibujar
                        lv_refr_now(NULL);          // Forzar redibujado
                        Serial.printf("[NAV] → Siguiente botón (focus en 0x%p)\n", focused);
                    } else if (key == 0xB5) { // UP
                        lv_group_focus_prev(grp);
                        lv_obj_t *focused = lv_group_get_focused(grp);
                        lv_obj_invalidate(focused); // Marcar como "dirty" para redibujar
                        lv_refr_now(NULL);          // Forzar redibujado
                        Serial.printf("[NAV] → Botón anterior (focus en 0x%p)\n", focused);
                    } else if (key == 0x0D) { // ENTER
                        lv_obj_t *focused = lv_group_get_focused(grp);
                        if (focused) {
                            lv_obj_send_event(focused, LV_EVENT_CLICKED, NULL);
                            Serial.println("[NAV] → ENTER presionado");
                        }
                    } else if (key == 0x1B) { // ESC - cerrar modal si está abierto
                        PanelManager::hideSettingEditor();
                        Serial.println("[NAV] → ESC presionado");
                    }
                }
            }
        }
    }

    delay(5);

    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 5000) {
        lastUpdate = millis();
        Serial.printf("[%lu s] Heap: %d | Pantalla: %d | Brillo: %d%%\n", millis() / 1000, ESP.getFreeHeap(),
                      ScreenManager::getCurrentScreen(), (display.getBrightness() * 100) / 255);
    }
}
