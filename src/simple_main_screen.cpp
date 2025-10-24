#include "simple_main_screen.h"
#include <Arduino.h>

extern "C" {
#include "images.h"
}

// Variable global para la pantalla
static lv_obj_t *main_screen = nullptr;

extern "C" void create_simple_main_screen()
{
    Serial.println("[SimpleUI] Creando main_screen simplificada");
    Serial.printf("[MEM] Free heap antes: %d bytes\n", ESP.getFreeHeap());

    // Crear pantalla base - Portrait 176x220
    main_screen = lv_obj_create(NULL);
    lv_obj_set_size(main_screen, 176, 220);
    lv_obj_set_style_bg_color(main_screen, lv_color_hex(0x1a1a1a), 0);
    lv_obj_clear_flag(main_screen, LV_OBJ_FLAG_SCROLLABLE);

    Serial.printf("[SimpleUI] Screen base creada\n");

    // Título grande
    lv_obj_t *title = lv_label_create(main_screen);
    lv_label_set_text(title, "MESHTASTIC\nDEVICE-UI");
    lv_obj_set_style_text_color(title, lv_color_hex(0x67ea94), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    Serial.printf("[SimpleUI] Título creado\n");

    // Información
    lv_obj_t *info = lv_label_create(main_screen);
    lv_label_set_text(info, "ILI9225 Real\n"
                            "176x220 Portrait\n\n"
                            "Nodos: 0\n"
                            "Mensajes: 0\n"
                            "Canal: LongFast\n\n"
                            "Heap: 281 KB");
    lv_obj_set_style_text_color(info, lv_color_hex(0xcccccc), 0);
    lv_obj_set_style_text_font(info, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_align(info, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(info, LV_ALIGN_CENTER, 0, 0);

    Serial.printf("[SimpleUI] Info creada\n");

    // Botón de prueba
    lv_obj_t *btn = lv_btn_create(main_screen);
    lv_obj_set_size(btn, 120, 40);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x67ea94), 0);

    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "OK");
    lv_obj_set_style_text_color(btn_label, lv_color_hex(0x000000), 0);
    lv_obj_center(btn_label);

    Serial.printf("[SimpleUI] Botón creado\n");

    Serial.printf("[MEM] Free heap después: %d bytes\n", ESP.getFreeHeap());
    Serial.println("[SimpleUI] Main screen simplificada creada");
}

extern "C" lv_obj_t *get_simple_main_screen()
{
    return main_screen;
}
