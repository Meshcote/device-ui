#include "panel_manager.h"
#include <Arduino.h>
#include <string.h>

extern "C" {
#include "ui.h"
}

// Forward declarations
static void setting_button_cb(lv_event_t *e);
static void modal_close_cb(lv_event_t *e);

// Variables estáticas
PanelID PanelManager::current_panel = PANEL_HOME;
lv_obj_t *PanelManager::main_container = nullptr;
lv_obj_t *PanelManager::home_panel = nullptr;
lv_obj_t *PanelManager::nodes_panel = nullptr;
lv_obj_t *PanelManager::messages_panel = nullptr;
lv_obj_t *PanelManager::map_panel = nullptr;
lv_obj_t *PanelManager::settings_panel = nullptr;
lv_obj_t *PanelManager::modal_dialog = nullptr;

void PanelManager::init(lv_obj_t *container)
{
    main_container = container;
    current_panel = PANEL_HOME;
}

void PanelManager::registerHomePanel(lv_obj_t *panel)
{
    home_panel = panel;
    Serial.printf("[PanelMgr] Home panel registrado: 0x%p\n", home_panel);
}

void PanelManager::showPanel(PanelID panel_id)
{
    Serial.printf("[PanelMgr] Cambiando de panel %d → %d\n", current_panel, panel_id);

    // Ocultar panel actual
    if (home_panel && current_panel == PANEL_HOME) {
        lv_obj_add_flag(home_panel, LV_OBJ_FLAG_HIDDEN);
    }
    if (nodes_panel && current_panel == PANEL_NODES) {
        lv_obj_add_flag(nodes_panel, LV_OBJ_FLAG_HIDDEN);
    }
    if (messages_panel && current_panel == PANEL_MESSAGES) {
        lv_obj_add_flag(messages_panel, LV_OBJ_FLAG_HIDDEN);
    }
    if (map_panel && current_panel == PANEL_MAP) {
        lv_obj_add_flag(map_panel, LV_OBJ_FLAG_HIDDEN);
    }
    if (settings_panel && current_panel == PANEL_SETTINGS) {
        lv_obj_add_flag(settings_panel, LV_OBJ_FLAG_HIDDEN);
    }

    // Mostrar nuevo panel
    switch (panel_id) {
    case PANEL_HOME:
        if (home_panel)
            lv_obj_clear_flag(home_panel, LV_OBJ_FLAG_HIDDEN);
        break;
    case PANEL_NODES:
        if (!nodes_panel) {
            Serial.println("[PanelMgr] Creando NODES panel...");
            nodes_panel = lv_obj_create(main_container);
            lv_obj_set_pos(nodes_panel, 30, 0);
            lv_obj_set_size(nodes_panel, 146, 220);
            lv_obj_set_style_bg_color(nodes_panel, lv_color_hex(0x0a0a0a), 0);
            lv_obj_set_style_border_width(nodes_panel, 0, 0);
            lv_obj_set_scrollbar_mode(nodes_panel, LV_SCROLLBAR_MODE_AUTO);
            lv_obj_set_scroll_dir(nodes_panel, LV_DIR_VER);
            lv_obj_set_flex_flow(nodes_panel, LV_FLEX_FLOW_COLUMN);
            lv_obj_set_style_pad_row(nodes_panel, 3, 0);

            // Título
            lv_obj_t *title = lv_label_create(nodes_panel);
            lv_label_set_text(title, "NODES");
            lv_obj_set_style_text_color(title, lv_color_hex(0x67ea94), 0);
            lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);

            // Nodo 1 - Este dispositivo
            lv_obj_t *node1 = lv_obj_create(nodes_panel);
            lv_obj_set_size(node1, 140, 45);
            lv_obj_set_style_bg_color(node1, lv_color_hex(0x1a1a1a), 0);
            lv_obj_set_style_border_color(node1, lv_color_hex(0x67ea94), 0);
            lv_obj_set_style_border_width(node1, 1, 0);
            lv_obj_set_style_radius(node1, 5, 0);
            lv_obj_clear_flag(node1, LV_OBJ_FLAG_SCROLLABLE);

            lv_obj_t *name1 = lv_label_create(node1);
            lv_label_set_text(name1, "!1234\nEste Nodo");
            lv_obj_set_style_text_font(name1, &lv_font_montserrat_10, 0);
            lv_obj_set_pos(name1, 5, 5);

            lv_obj_t *info1 = lv_label_create(node1);
            lv_label_set_text(info1, "100% 0m");
            lv_obj_set_style_text_font(info1, &lv_font_montserrat_10, 0);
            lv_obj_set_style_text_color(info1, lv_color_hex(0x888888), 0);
            lv_obj_align(info1, LV_ALIGN_TOP_RIGHT, -5, 5);

            // Nodo 2 - Ejemplo offline
            lv_obj_t *node2 = lv_obj_create(nodes_panel);
            lv_obj_set_size(node2, 140, 45);
            lv_obj_set_style_bg_color(node2, lv_color_hex(0x1a1a1a), 0);
            lv_obj_set_style_border_color(node2, lv_color_hex(0x444444), 0);
            lv_obj_set_style_border_width(node2, 1, 0);
            lv_obj_set_style_radius(node2, 5, 0);
            lv_obj_clear_flag(node2, LV_OBJ_FLAG_SCROLLABLE);

            lv_obj_t *name2 = lv_label_create(node2);
            lv_label_set_text(name2, "!5678\nOtro Nodo");
            lv_obj_set_style_text_font(name2, &lv_font_montserrat_10, 0);
            lv_obj_set_style_text_color(name2, lv_color_hex(0x888888), 0);
            lv_obj_set_pos(name2, 5, 5);

            lv_obj_t *info2 = lv_label_create(node2);
            lv_label_set_text(info2, "? 5h");
            lv_obj_set_style_text_font(info2, &lv_font_montserrat_10, 0);
            lv_obj_set_style_text_color(info2, lv_color_hex(0x666666), 0);
            lv_obj_align(info2, LV_ALIGN_TOP_RIGHT, -5, 5);

            Serial.printf("[PanelMgr] NODES panel creado: 0x%p\n", nodes_panel);
        }
        if (nodes_panel)
            lv_obj_clear_flag(nodes_panel, LV_OBJ_FLAG_HIDDEN);
        break;
    case PANEL_MESSAGES:
        if (!messages_panel) {
            Serial.println("[PanelMgr] Creando MESSAGES panel...");
            messages_panel = lv_obj_create(main_container);
            lv_obj_set_pos(messages_panel, 30, 0);
            lv_obj_set_size(messages_panel, 146, 220);
            lv_obj_set_style_bg_color(messages_panel, lv_color_hex(0x0a0a0a), 0);
            lv_obj_set_style_border_width(messages_panel, 0, 0);
            lv_obj_set_flex_flow(messages_panel, LV_FLEX_FLOW_COLUMN);
            lv_obj_set_style_pad_row(messages_panel, 3, 0);

            // Título
            lv_obj_t *title = lv_label_create(messages_panel);
            lv_label_set_text(title, "MESSAGES");
            lv_obj_set_style_text_color(title, lv_color_hex(0x67ea94), 0);
            lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);

            // Mensaje 1
            lv_obj_t *msg1 = lv_obj_create(messages_panel);
            lv_obj_set_size(msg1, 140, 50);
            lv_obj_set_style_bg_color(msg1, lv_color_hex(0x1a3a1a), 0);
            lv_obj_set_style_border_width(msg1, 0, 0);
            lv_obj_set_style_radius(msg1, 5, 0);
            lv_obj_clear_flag(msg1, LV_OBJ_FLAG_SCROLLABLE);

            lv_obj_t *from1 = lv_label_create(msg1);
            lv_label_set_text(from1, "!1234 12:34");
            lv_obj_set_style_text_font(from1, &lv_font_montserrat_10, 0);
            lv_obj_set_style_text_color(from1, lv_color_hex(0x67ea94), 0);
            lv_obj_set_pos(from1, 5, 3);

            lv_obj_t *text1 = lv_label_create(msg1);
            lv_label_set_text(text1, "Hola! Como estas?");
            lv_obj_set_style_text_font(text1, &lv_font_montserrat_10, 0);
            lv_obj_set_pos(text1, 5, 18);
            lv_obj_set_width(text1, 130);
            lv_label_set_long_mode(text1, LV_LABEL_LONG_WRAP);

            // Info vacío
            lv_obj_t *empty = lv_label_create(messages_panel);
            lv_label_set_text(empty, "\nNo hay mas\nmensajes");
            lv_obj_set_style_text_color(empty, lv_color_hex(0x666666), 0);
            lv_obj_set_style_text_font(empty, &lv_font_montserrat_10, 0);
            lv_obj_align(empty, LV_ALIGN_CENTER, 0, 20);

            Serial.printf("[PanelMgr] MESSAGES panel creado: 0x%p\n", messages_panel);
        }
        if (messages_panel)
            lv_obj_clear_flag(messages_panel, LV_OBJ_FLAG_HIDDEN);
        break;
    case PANEL_MAP:
        if (!map_panel) {
            Serial.println("[PanelMgr] Creando MAP panel...");
            map_panel = lv_obj_create(main_container);
            lv_obj_set_pos(map_panel, 30, 0);
            lv_obj_set_size(map_panel, 146, 220);
            lv_obj_set_style_bg_color(map_panel, lv_color_hex(0x0a0a0a), 0);
            lv_obj_set_style_border_width(map_panel, 0, 0);
            lv_obj_clear_flag(map_panel, LV_OBJ_FLAG_SCROLLABLE);

            // Título
            lv_obj_t *title = lv_label_create(map_panel);
            lv_label_set_text(title, "MAP");
            lv_obj_set_style_text_color(title, lv_color_hex(0x67ea94), 0);
            lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
            lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5);

            // Info (mapa real requiere demasiada RAM)
            lv_obj_t *info = lv_label_create(map_panel);
            lv_label_set_text(info, "\n\n"
                                    "Posicion:\n"
                                    "GPS: No fijado\n\n"
                                    "Altitud: --m\n"
                                    "Satelites: 0\n\n"
                                    "Mapa completo\n"
                                    "requiere mas RAM");
            lv_obj_set_style_text_color(info, lv_color_hex(0xaaaaaa), 0);
            lv_obj_set_style_text_font(info, &lv_font_montserrat_10, 0);
            lv_obj_set_style_text_align(info, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(info, LV_ALIGN_CENTER, 0, 0);

            Serial.printf("[PanelMgr] MAP panel creado: 0x%p\n", map_panel);
        }
        if (map_panel)
            lv_obj_clear_flag(map_panel, LV_OBJ_FLAG_HIDDEN);
        break;
    case PANEL_SETTINGS:
        if (!settings_panel) {
            Serial.println("[PanelMgr] Creando SETTINGS panel...");
            settings_panel = lv_obj_create(main_container);
            lv_obj_set_pos(settings_panel, 30, 0);
            lv_obj_set_size(settings_panel, 146, 220);
            lv_obj_set_style_bg_color(settings_panel, lv_color_hex(0x0a0a0a), 0);
            lv_obj_set_style_border_width(settings_panel, 0, 0);
            lv_obj_set_scrollbar_mode(settings_panel, LV_SCROLLBAR_MODE_AUTO);
            lv_obj_set_scroll_dir(settings_panel, LV_DIR_VER);
            lv_obj_set_flex_flow(settings_panel, LV_FLEX_FLOW_COLUMN);
            lv_obj_set_style_pad_row(settings_panel, 5, 0);

            // Título
            lv_obj_t *title = lv_label_create(settings_panel);
            lv_label_set_text(title, "SETTINGS");
            lv_obj_set_style_text_color(title, lv_color_hex(0x67ea94), 0);
            lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);

            // Crear botones con sus valores actuales
            struct SettingItem {
                const char *label;
                const char *value;
            };

            SettingItem settings[] = {{"Usuario", "Meshtastic"},    {"Region", "EU_868"},       {"Canal", "LongFast"},
                                      {"LoRa Preset", "LONG_FAST"}, {"Display", "Brillo 100%"}, {"WiFi", "Desactivado"},
                                      {"Bluetooth", "Activado"},    {"GPS", "Buscando..."},     {"Reboot", "Reiniciar"}};

            for (int i = 0; i < 9; i++) {
                lv_obj_t *btn = lv_btn_create(settings_panel);
                lv_obj_set_size(btn, 135, 35);
                lv_obj_set_style_bg_color(btn, lv_color_hex(0x1a1a1a), 0);
                lv_obj_set_style_border_width(btn, 1, 0);
                lv_obj_set_style_border_color(btn, lv_color_hex(0x444444), 0);

                lv_obj_t *label = lv_label_create(btn);
                char text[64];
                snprintf(text, sizeof(text), "%s:\n%s", settings[i].label, settings[i].value);
                lv_label_set_text(label, text);
                lv_obj_set_style_text_font(label, &lv_font_montserrat_10, 0);
                lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

                // Event handler con ID del setting
                lv_obj_add_event_cb(btn, setting_button_cb, LV_EVENT_CLICKED, (void *)(intptr_t)i);
            }

            Serial.printf("[PanelMgr] SETTINGS panel creado: 0x%p\n", settings_panel);
        }
        if (settings_panel)
            lv_obj_clear_flag(settings_panel, LV_OBJ_FLAG_HIDDEN);
        break;
    }

    current_panel = panel_id;
    lv_refr_now(NULL);
}

PanelID PanelManager::getCurrentPanel()
{
    return current_panel;
}

// Callback para cerrar modal (ESC o botón Cancelar)
static void modal_close_cb(lv_event_t *e)
{
    PanelManager::hideSettingEditor();
}

// Callback para botones de Settings
static void setting_button_cb(lv_event_t *e)
{
    int setting_id = (int)(intptr_t)lv_event_get_user_data(e);
    lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);

    // Obtener el label del botón para extraer el valor actual
    lv_obj_t *label = lv_obj_get_child(btn, 0);
    const char *text = lv_label_get_text(label);

    // Extraer título y valor (formato "Titulo:\nValor")
    char title[32] = {0};
    char value[32] = {0};
    const char *newline = strchr(text, '\n');
    if (newline) {
        size_t title_len = newline - text;
        if (title_len > sizeof(title) - 1)
            title_len = sizeof(title) - 1;
        strncpy(title, text, title_len);
        title[title_len] = '\0';
        // Remover el ':'
        char *colon = strchr(title, ':');
        if (colon)
            *colon = '\0';

        strncpy(value, newline + 1, sizeof(value) - 1);
    }

    Serial.printf("[Settings] Click en boton %d: %s\n", setting_id, title);
    PanelManager::showSettingEditor(title, value, setting_id);
}

void PanelManager::showSettingEditor(const char *title, const char *currentValue, int settingId)
{
    if (modal_dialog) {
        lv_obj_del(modal_dialog);
        modal_dialog = nullptr;
    }

    Serial.printf("[PanelMgr] Mostrando editor: %s = %s\n", title, currentValue);

    // Fondo oscuro semi-transparente
    modal_dialog = lv_obj_create(lv_scr_act());
    lv_obj_set_size(modal_dialog, 176, 220);
    lv_obj_set_pos(modal_dialog, 0, 0);
    lv_obj_set_style_bg_color(modal_dialog, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(modal_dialog, LV_OPA_80, 0);
    lv_obj_set_style_border_width(modal_dialog, 0, 0);

    // Caja del diálogo
    lv_obj_t *box = lv_obj_create(modal_dialog);
    lv_obj_set_size(box, 160, 140);
    lv_obj_center(box);
    lv_obj_set_style_bg_color(box, lv_color_hex(0x1a1a1a), 0);
    lv_obj_set_style_border_width(box, 2, 0);
    lv_obj_set_style_border_color(box, lv_color_hex(0x67ea94), 0);

    // Título
    lv_obj_t *title_label = lv_label_create(box);
    lv_label_set_text(title_label, title);
    lv_obj_set_style_text_color(title_label, lv_color_hex(0x67ea94), 0);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_14, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 5);

    // Dependiendo del tipo de setting, mostrar diferentes controles
    switch (settingId) {
    case 0: { // Usuario - text input
        lv_obj_t *ta = lv_textarea_create(box);
        lv_obj_set_size(ta, 140, 40);
        lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 30);
        lv_textarea_set_text(ta, currentValue);
        lv_textarea_set_max_length(ta, 20);
        break;
    }
    case 1: { // Region - dropdown
        lv_obj_t *dd = lv_dropdown_create(box);
        lv_dropdown_set_options(dd, "EU_868\nUS\nEU_433\nAU_915\nNZ_865\nCN");
        lv_obj_set_size(dd, 140, 40);
        lv_obj_align(dd, LV_ALIGN_TOP_MID, 0, 30);
        break;
    }
    case 8: { // Reboot - confirmación
        lv_obj_t *warning = lv_label_create(box);
        lv_label_set_text(warning, "\n\nReiniciar\ndispositivo?");
        lv_obj_set_style_text_color(warning, lv_color_hex(0xff6666), 0);
        lv_obj_set_style_text_align(warning, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_center(warning);
        break;
    }
    default: { // Otros - mostrar valor actual
        lv_obj_t *value_label = lv_label_create(box);
        lv_label_set_text(value_label, currentValue);
        lv_obj_set_style_text_color(value_label, lv_color_hex(0xcccccc), 0);
        lv_obj_set_style_text_align(value_label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align(value_label, LV_ALIGN_CENTER, 0, -10);
        break;
    }
    }

    // Botón Cancelar
    lv_obj_t *btn_cancel = lv_btn_create(box);
    lv_obj_set_size(btn_cancel, 60, 30);
    lv_obj_align(btn_cancel, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    lv_obj_add_event_cb(btn_cancel, modal_close_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_label = lv_label_create(btn_cancel);
    lv_label_set_text(btn_label, "Salir");
    lv_obj_center(btn_label);

    // Botón Aceptar
    lv_obj_t *btn_ok = lv_btn_create(box);
    lv_obj_set_size(btn_ok, 60, 30);
    lv_obj_align(btn_ok, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
    lv_obj_set_style_bg_color(btn_ok, lv_color_hex(0x4db270), 0);
    // TODO: Callback para guardar valor

    btn_label = lv_label_create(btn_ok);
    lv_label_set_text(btn_label, "OK");
    lv_obj_center(btn_label);

    lv_refr_now(NULL);
}

void PanelManager::hideSettingEditor()
{
    if (modal_dialog) {
        Serial.println("[PanelMgr] Cerrando modal dialog");
        lv_obj_del(modal_dialog);
        modal_dialog = nullptr;
        lv_refr_now(NULL);
    }
}
