// Configuración LVGL simplificada para dispositivos sin PSRAM

#ifndef LV_CONF_H
#define LV_CONF_H

// Profundidad de color
#define LV_COLOR_DEPTH 16

// Memoria
#define LV_MEM_SIZE (48 * 1024U)

// Sin logs en producción para ahorrar memoria
#define LV_USE_LOG 0

// Fuentes básicas
#define LV_FONT_MONTSERRAT_8 0
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 0
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0

// Fuente por defecto
#define LV_FONT_DEFAULT &lv_font_montserrat_14

// Widgets esenciales
#define LV_USE_LABEL 1
#define LV_USE_BUTTON 1
#define LV_USE_IMAGE 1
#define LV_USE_BAR 1
#define LV_USE_ARC 1
#define LV_USE_LINE 1

// Widgets opcionales (desactivados para ahorrar memoria)
#define LV_USE_SPINNER 0
#define LV_USE_CHART 0
#define LV_USE_TABLE 0
#define LV_USE_TEXTAREA 0
#define LV_USE_CALENDAR 0
#define LV_USE_COLORWHEEL 0
#define LV_USE_IMGBTN 0
#define LV_USE_KEYBOARD 0
#define LV_USE_LED 0
#define LV_USE_LIST 0
#define LV_USE_MENU 0
#define LV_USE_METER 0
#define LV_USE_MSGBOX 0
#define LV_USE_ROLLER 0
#define LV_USE_SLIDER 0
#define LV_USE_SPAN 0
#define LV_USE_SPINBOX 0
#define LV_USE_SWITCH 0
#define LV_USE_TABVIEW 0
#define LV_USE_TILEVIEW 0
#define LV_USE_WIN 0

// Temas
#define LV_USE_THEME_DEFAULT 1
#define LV_USE_THEME_MONO 0

// Animaciones
#define LV_USE_ANIMATION 1

// Caché de imágenes reducido
#define LV_IMG_CACHE_DEF_SIZE 1

// Performance
#define LV_USE_PERF_MONITOR 0
#define LV_USE_MEM_MONITOR 0
#define LV_USE_REFR_DEBUG 0

// Tick
#define LV_TICK_CUSTOM 1
#if LV_TICK_CUSTOM
#define LV_TICK_CUSTOM_INCLUDE "Arduino.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())
#endif

#endif // LV_CONF_H
