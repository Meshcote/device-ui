#include "progressive_loader.h"
#include "input/InputDriver.h"
#include "panel_manager.h"
#include "screen_manager.h"
#include <Arduino.h>

extern "C" {
#include "images.h"
#include "screens.h"
#include "styles.h"
#include "ui.h"
}

// Event handlers para los botones de navegación
static void home_button_clicked(lv_event_t *e)
{
    Serial.println("[NAV] ★★★ HOME BUTTON CLICKED ★★★");
    PanelManager::showPanel(PANEL_HOME);
}

static void nodes_button_clicked(lv_event_t *e)
{
    Serial.println("[NAV] ★★★ NODES BUTTON CLICKED ★★★");
    PanelManager::showPanel(PANEL_NODES);
}

static void messages_button_clicked(lv_event_t *e)
{
    Serial.println("[NAV] ★★★ MESSAGES BUTTON CLICKED ★★★");
    PanelManager::showPanel(PANEL_MESSAGES);
}

static void map_button_clicked(lv_event_t *e)
{
    Serial.println("[NAV] ★★★ MAP BUTTON CLICKED ★★★");
    PanelManager::showPanel(PANEL_MAP);
}

static void settings_button_clicked(lv_event_t *e)
{
    Serial.println("[NAV] ★★★ SETTINGS BUTTON CLICKED ★★★");
    PanelManager::showPanel(PANEL_SETTINGS);
}

// Cargar main_screen de forma progresiva para evitar colapso de RAM
extern "C" bool create_main_screen_progressive()
{
    Serial.println("[ProgLoader] Iniciando carga progresiva de main_screen");

    uint32_t initial_heap = ESP.getFreeHeap();
    Serial.printf("[ProgLoader] Heap inicial: %d bytes\n", initial_heap);

    // PASO 1: Crear pantalla base (muy pequeño)
    Serial.println("[ProgLoader] PASO 1: Creando pantalla base...");
    lv_obj_t *obj = lv_obj_create(0);
    if (!obj) {
        Serial.println("[ProgLoader] ERROR: No se pudo crear pantalla base");
        return false;
    }
    objects.main_screen = obj;
    lv_obj_set_size(obj, 176, 220); // Portrait
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

    // Fondo negro (UI normal)
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), 0);

    Serial.printf("[ProgLoader] Heap después paso 1: %d bytes (usados: %d)\n", ESP.getFreeHeap(),
                  initial_heap - ESP.getFreeHeap());

    // PASO 2: Crear solo el panel de botones lateral (crítico)
    Serial.println("[ProgLoader] PASO 2: Creando panel de botones...");
    lv_obj_t *button_panel = lv_obj_create(obj);
    if (!button_panel) {
        Serial.println("[ProgLoader] ERROR: No se pudo crear panel de botones");
        return false;
    }
    objects.button_panel = button_panel;
    lv_obj_set_pos(button_panel, 0, 0);
    lv_obj_set_size(button_panel, 30, 220);
    lv_obj_set_style_bg_color(button_panel, lv_color_hex(0x1a1a1a), 0);
    lv_obj_set_style_border_width(button_panel, 0, 0);
    lv_obj_clear_flag(button_panel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(button_panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(button_panel, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(button_panel, 4, 0);

    Serial.printf("[ProgLoader] Heap después paso 2: %d bytes (usados: %d)\n", ESP.getFreeHeap(),
                  initial_heap - ESP.getFreeHeap());

    if (ESP.getFreeHeap() < 200000) {
        Serial.println("[ProgLoader] ADVERTENCIA: Memoria baja, simplificando...");
        // Crear solo texto en lugar de imágenes
        lv_obj_t *label = lv_label_create(obj);
        lv_label_set_text(label, "MESHTASTIC\nDevice-UI\n\nSin PSRAM\nPantalla\nSimplificada");
        lv_obj_set_style_text_color(label, lv_color_hex(0x67ea94), 0);
        lv_obj_center(label);
        return true;
    }

    // PASO 3: Crear botones uno por uno, verificando memoria
    Serial.println("[ProgLoader] PASO 3: Creando botones...");

    // Botón HOME
    lv_obj_t *home_btn = lv_btn_create(button_panel);
    objects.home_button = home_btn;
    lv_obj_set_size(home_btn, 28, 28);
    lv_obj_set_style_bg_image_src(home_btn, &img_home_button_image, 0);
    lv_obj_set_style_image_recolor(home_btn, lv_color_hex(0x67ea94), 0);
    lv_obj_set_style_image_recolor_opa(home_btn, 255, 0);
    lv_obj_add_event_cb(home_btn, home_button_clicked, LV_EVENT_CLICKED, NULL);
    Serial.printf("[ProgLoader]   Home button - Heap: %d\n", ESP.getFreeHeap());

    // Botón NODES
    lv_obj_t *nodes_btn = lv_btn_create(button_panel);
    objects.nodes_button = nodes_btn;
    lv_obj_set_size(nodes_btn, 28, 28);
    lv_obj_set_style_bg_image_src(nodes_btn, &img_nodes_button_image, 0);
    lv_obj_add_event_cb(nodes_btn, nodes_button_clicked, LV_EVENT_CLICKED, NULL);
    Serial.printf("[ProgLoader]   Nodes button - Heap: %d\n", ESP.getFreeHeap());

    // Botón MESSAGES
    lv_obj_t *msg_btn = lv_btn_create(button_panel);
    objects.messages_button = msg_btn;
    lv_obj_set_size(msg_btn, 28, 28);
    lv_obj_set_style_bg_image_src(msg_btn, &img_messages_button_image, 0);
    lv_obj_add_event_cb(msg_btn, messages_button_clicked, LV_EVENT_CLICKED, NULL);
    Serial.printf("[ProgLoader]   Messages button - Heap: %d\n", ESP.getFreeHeap());

    // Botón MAP
    lv_obj_t *map_btn = lv_btn_create(button_panel);
    objects.map_button = map_btn;
    lv_obj_set_size(map_btn, 28, 28);
    lv_obj_set_style_bg_image_src(map_btn, &img_map_button_image, 0);
    lv_obj_add_event_cb(map_btn, map_button_clicked, LV_EVENT_CLICKED, NULL);
    Serial.printf("[ProgLoader]   Map button - Heap: %d\n", ESP.getFreeHeap());

    // Botón SETTINGS
    lv_obj_t *settings_btn = lv_btn_create(button_panel);
    objects.settings_button = settings_btn;
    lv_obj_set_size(settings_btn, 28, 28);
    lv_obj_set_style_bg_image_src(settings_btn, &img_settings_button_image, 0);
    lv_obj_add_event_cb(settings_btn, settings_button_clicked, LV_EVENT_CLICKED, NULL);
    Serial.printf("[ProgLoader]   Settings button - Heap: %d\n", ESP.getFreeHeap());

    // PASO 4: Crear panel HOME simple (sin todos los detalles)
    Serial.println("[ProgLoader] PASO 4: Creando panel HOME básico...");
    lv_obj_t *home_panel = lv_obj_create(obj);
    objects.home_panel = home_panel;
    lv_obj_set_pos(home_panel, 30, 0);
    lv_obj_set_size(home_panel, 146, 220);
    lv_obj_set_style_bg_color(home_panel, lv_color_hex(0x0a0a0a), 0);
    lv_obj_set_style_border_width(home_panel, 0, 0);
    lv_obj_clear_flag(home_panel, LV_OBJ_FLAG_SCROLLABLE);

    // Agregar solo información básica
    lv_obj_t *info_label = lv_label_create(home_panel);
    lv_label_set_text(info_label, "MESHTASTIC\n"
                                  "Device-UI\n\n"
                                  "ILI9225 176x220\n"
                                  "Sin PSRAM\n\n"
                                  "Usa flechas\n"
                                  "para navegar");
    lv_obj_set_style_text_color(info_label, lv_color_hex(0xcccccc), 0);
    lv_obj_set_style_text_font(info_label, &lv_font_montserrat_12, 0);
    lv_obj_align(info_label, LV_ALIGN_CENTER, 0, 0);

    // Inicializar PanelManager con el home_panel
    PanelManager::init(obj); // obj es la pantalla principal
    PanelManager::registerHomePanel(home_panel);

    uint32_t final_heap = ESP.getFreeHeap();
    Serial.printf("[ProgLoader] Carga completa - Heap final: %d bytes (usados: %d)\n", final_heap, initial_heap - final_heap);

    // Obtener grupo de navegación del CardKB (si existe)
    lv_group_t *kb_group = InputDriver::getInputGroup();
    Serial.printf("[ProgLoader DEBUG] InputDriver::getInputGroup() = 0x%p\n", kb_group);

    if (kb_group) {
        Serial.println("[ProgLoader] Configurando navegación por teclado...");

        // Hacer los botones navegables (clickables)
        lv_obj_clear_flag(home_btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);
        lv_obj_add_flag(home_btn, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(nodes_btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);
        lv_obj_add_flag(nodes_btn, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(msg_btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);
        lv_obj_add_flag(msg_btn, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(map_btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);
        lv_obj_add_flag(map_btn, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(settings_btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);
        lv_obj_add_flag(settings_btn, LV_OBJ_FLAG_CLICKABLE);

        // Estilo visual para el botón con FOCUS (borde verde brillante)
        static lv_style_t style_focused;
        lv_style_init(&style_focused);
        lv_style_set_outline_width(&style_focused, 3);
        lv_style_set_outline_color(&style_focused, lv_color_hex(0x00FF00));
        lv_style_set_outline_opa(&style_focused, LV_OPA_COVER);

        // Aplicar estilo a todos los botones cuando tengan focus
        lv_obj_add_style(home_btn, &style_focused, LV_STATE_FOCUSED);
        lv_obj_add_style(nodes_btn, &style_focused, LV_STATE_FOCUSED);
        lv_obj_add_style(msg_btn, &style_focused, LV_STATE_FOCUSED);
        lv_obj_add_style(map_btn, &style_focused, LV_STATE_FOCUSED);
        lv_obj_add_style(settings_btn, &style_focused, LV_STATE_FOCUSED);

        // Agregar botones al grupo para navegación
        lv_group_add_obj(kb_group, home_btn);
        lv_group_add_obj(kb_group, nodes_btn);
        lv_group_add_obj(kb_group, msg_btn);
        lv_group_add_obj(kb_group, map_btn);
        lv_group_add_obj(kb_group, settings_btn);

        Serial.printf("[ProgLoader DEBUG] Grupo tiene %d objetos\n", lv_group_get_obj_count(kb_group));

        // Focus en el primer botón (Home)
        lv_group_focus_obj(home_btn);
        lv_obj_t *focused = lv_group_get_focused(kb_group);
        Serial.printf("[ProgLoader DEBUG] Objeto con focus: 0x%p (esperado: 0x%p)\n", focused, home_btn);

        Serial.println("[ProgLoader] ✓ 5 botones agregados al grupo de navegación");
    } else {
        Serial.println("[ProgLoader] ERROR: Sin grupo de navegación!");
    }

    Serial.println("[ProgLoader] Main screen cargada exitosamente!");
    return true;
}
