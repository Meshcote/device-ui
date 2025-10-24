#include "graphics/LVGL/LVGLGraphics.h"
#include "assert.h"
#include "util/ILog.h"

#ifdef ILI9225_DISPLAY
#include "graphics/ILI9225Display.h"

// Instancia global del display ILI9225
static ILI9225Display *ili9225_display = nullptr;
#endif

LVGLGraphics::LVGLGraphics(uint16_t width, uint16_t height) : screenWidth(width), screenHeight(height) {}

void LVGLGraphics::init(void)
{
    ILOG_DEBUG("LV init...");

#ifdef ILI9225_DISPLAY
    ILOG_INFO("Initializing ILI9225 Display (No PSRAM mode)");
    ILOG_INFO("Display size: %dx%d", screenWidth, screenHeight);
#endif

#if LV_USE_LOG
    lv_log_register_print_cb(lv_debug);
#endif
    lv_init();
#if LV_USE_LOG
    lv_log_register_print_cb(lv_debug);
#endif

#ifdef ILI9225_DISPLAY
    // Crear e inicializar display ILI9225 con pines del usuario:
    // CS=7, DC=5, RST=6, MOSI=4, SCLK=3, BL=2
    ili9225_display = new ILI9225Display(7, 5, 6, 4, 3, 2);

    if (!ili9225_display->init()) {
        ILOG_ERROR("Failed to initialize ILI9225 display!");
        return;
    }

    ILOG_INFO("ILI9225 Display initialized successfully");
#endif
}

// debugging callback
void LVGLGraphics::lv_debug(lv_log_level_t level, const char *buf)
{
    switch (level) {
    case LV_LOG_LEVEL_TRACE: {
        ILOG_DEBUG("%s", buf);
        break;
    }
    case LV_LOG_LEVEL_INFO: {
        ILOG_INFO("%s", buf);
        break;
    }
    case LV_LOG_LEVEL_WARN: {
        ILOG_WARN("%s", buf);
        break;
    }
    case LV_LOG_LEVEL_ERROR: {
        ILOG_ERROR("%s", buf);
        break;
    }
    default:
        ILOG_DEBUG("%s", buf);
        break;
    }
}
