#ifndef PANEL_MANAGER_H
#define PANEL_MANAGER_H

#include "lvgl.h"

enum PanelID { PANEL_HOME = 0, PANEL_NODES, PANEL_MESSAGES, PANEL_MAP, PANEL_SETTINGS };

class PanelManager
{
  public:
    static void init(lv_obj_t *container);
    static void registerHomePanel(lv_obj_t *panel);
    static void showPanel(PanelID panel_id);
    static PanelID getCurrentPanel();

    // Settings modal dialog
    static void showSettingEditor(const char *title, const char *currentValue, int settingId);
    static void hideSettingEditor();

  private:
    static PanelID current_panel;
    static lv_obj_t *main_container;
    static lv_obj_t *home_panel;
    static lv_obj_t *nodes_panel;
    static lv_obj_t *messages_panel;
    static lv_obj_t *map_panel;
    static lv_obj_t *settings_panel;
    static lv_obj_t *modal_dialog;
};

#endif // PANEL_MANAGER_H
