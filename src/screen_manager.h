#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "lvgl.h"

enum ScreenID { SCREEN_NONE = 0, SCREEN_BOOT, SCREEN_MAIN, SCREEN_LOCK, SCREEN_CALIBRATION, SCREEN_SETTINGS };

class ScreenManager
{
  public:
    static void init();
    static void loadScreen(ScreenID screen_id);
    static void tick();
    static ScreenID getCurrentScreen();
};

#endif // SCREEN_MANAGER_H
