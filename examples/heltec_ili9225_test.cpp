#include "graphics/LVGL/LVGLGraphics.h"
#include <Arduino.h>

LVGLGraphics *graphics = nullptr;
unsigned long lastMemCheck = 0;

void createTestUI()
{
    // Pantalla principal
    lv_obj_t *screen = lv_screen_active();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), 0);

    // Logo/Título
    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "MESHTASTIC");
    lv_obj_set_style_text_color(title, lv_color_hex(0x00FF00), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // Información del dispositivo
    lv_obj_t *info = lv_label_create(screen);
    lv_label_set_text(info, "Heltec WiFi LoRa V3\nILI9225 176x220\nSin PSRAM");
    lv_obj_set_style_text_align(info, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(info, LV_ALIGN_CENTER, 0, 0);

    // Barra de progreso de prueba
    lv_obj_t *bar = lv_bar_create(screen);
    lv_obj_set_size(bar, 150, 20);
    lv_obj_align(bar, LV_ALIGN_BOTTOM_MID, 0, -30);
    lv_bar_set_value(bar, 75, LV_ANIM_OFF);

    // Label de memoria
    lv_obj_t *mem_label = lv_label_create(screen);
    lv_label_set_text_fmt(mem_label, "Free: %d KB", ESP.getFreeHeap() / 1024);
    lv_obj_set_style_text_color(mem_label, lv_color_hex(0xFFFF00), 0);
    lv_obj_align(mem_label, LV_ALIGN_BOTTOM_MID, 0, -10);
}

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println("\n\n╔═══════════════════════════════════════╗");
    Serial.println("║   Heltec WiFi LoRa V3 + ILI9225      ║");
    Serial.println("║   Test sin PSRAM                      ║");
    Serial.println("╚═══════════════════════════════════════╝\n");

    Serial.printf("Chip: %s\n", ESP.getChipModel());
    Serial.printf("CPU Freq: %d MHz\n", ESP.getCpuFreqMHz());
    Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("PSRAM: %d bytes (debe ser 0)\n\n", ESP.getFreePsram());

    // Inicializar gráficos con tamaño ILI9225
    graphics = new LVGLGraphics(176, 220);
    graphics->init();

    // Crear UI de prueba
    createTestUI();

    Serial.println("\n=== Sistema listo ===");
    Serial.printf("Free heap final: %d bytes\n", ESP.getFreeHeap());
}

void loop()
{
    // Actualizar LVGL (debe llamarse frecuentemente)
    lv_timer_handler();

    // Mostrar memoria cada 5 segundos
    if (millis() - lastMemCheck > 5000) {
        lastMemCheck = millis();
        Serial.printf("[%lu] Free heap: %d bytes\n", millis() / 1000, ESP.getFreeHeap());
    }

    delay(5);
}
