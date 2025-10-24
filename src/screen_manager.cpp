#include "screen_manager.h"
#include "progressive_loader.h"
#include "simple_main_screen.h"
#include <Arduino.h>

extern "C" {
#include "screens.h"
#include "ui.h"
}

// Pantalla actualmente cargada
static lv_obj_t *current_screen = nullptr;
static ScreenID current_screen_id = SCREEN_NONE;

void ScreenManager::init()
{
    Serial.println("[ScreenMgr] Inicializando gestor de pantallas");

    // Cargar pantalla de boot primero (logo Meshtastic)
    loadScreen(SCREEN_BOOT);

    // Después de 3 segundos, cambiar a main_screen
    // (En una implementación real, esto se haría cuando el sistema esté listo)
}

void ScreenManager::loadScreen(ScreenID screen_id)
{
    Serial.printf("[ScreenMgr] Cargando pantalla %d\n", screen_id);
    Serial.printf("[MEM] Free heap antes: %d bytes\n", ESP.getFreeHeap());

    // DESTRUIR pantalla anterior para liberar RAM (como dijiste)
    if (current_screen != nullptr && current_screen_id != screen_id) {
        Serial.printf("[ScreenMgr] DESTRUYENDO pantalla anterior %d para liberar RAM\n", current_screen_id);
        uint32_t heap_before_delete = ESP.getFreeHeap();

        lv_obj_del(current_screen);
        current_screen = nullptr;

        uint32_t heap_after_delete = ESP.getFreeHeap();
        Serial.printf("[MEM] RAM liberada: %d bytes\n", heap_after_delete - heap_before_delete);
        Serial.printf("[MEM] Free heap después de destruir: %d bytes\n", heap_after_delete);
    }

    // Crear solo la pantalla solicitada
    switch (screen_id) {
    case SCREEN_BOOT:
        Serial.println("[ScreenMgr] Creando boot_screen");
        create_screen_boot_screen();
        current_screen = objects.boot_screen;
        break;

    case SCREEN_MAIN:
        Serial.println("[ScreenMgr] Creando main_screen REAL con carga progresiva");
        if (create_main_screen_progressive()) {
            current_screen = objects.main_screen;
        } else {
            Serial.println("[ScreenMgr] ERROR: Fallo al crear main_screen");
            return;
        }
        break;

    case SCREEN_LOCK:
        Serial.println("[ScreenMgr] Creando lock_screen");
        create_screen_lock_screen();
        current_screen = objects.lock_screen;
        break;

    case SCREEN_CALIBRATION:
        Serial.println("[ScreenMgr] Creando calibration_screen");
        create_screen_calibration_screen();
        current_screen = objects.calibration_screen;
        break;

    case SCREEN_SETTINGS:
        Serial.println("[ScreenMgr] Creando settings");
        create_tabview_settings();
        // Settings usa el main_screen como base
        current_screen = objects.main_screen;
        break;

    default:
        Serial.printf("[ScreenMgr] ERROR: Pantalla %d no reconocida\n", screen_id);
        return;
    }

    current_screen_id = screen_id;

    // Cargar la pantalla en LVGL
    if (current_screen != nullptr) {
        Serial.printf("[ScreenMgr] Llamando lv_screen_load(0x%p)\n", current_screen);
        lv_screen_load(current_screen);

        // FORZAR actualización inmediata para debug
        lv_refr_now(NULL);
        Serial.println("[ScreenMgr] lv_refr_now() ejecutado - pantalla debería verse AHORA");
        Serial.println("[ScreenMgr] Pantalla cargada");
    } else {
        Serial.println("[ScreenMgr] ERROR: current_screen es NULL!");
    }

    Serial.printf("[MEM] Free heap después de cargar: %d bytes\n", ESP.getFreeHeap());
}

void ScreenManager::tick()
{
    // Procesar eventos de LVGL
    lv_timer_handler();
}

ScreenID ScreenManager::getCurrentScreen()
{
    return current_screen_id;
}
