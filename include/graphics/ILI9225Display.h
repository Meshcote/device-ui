#pragma once

#include "lvgl.h"
#include <Arduino.h>
#include <SPI.h>

// Configuración para ILI9225 176x220
#define ILI9225_WIDTH 176
#define ILI9225_HEIGHT 220

// Buffer reducido para dispositivos sin PSRAM
// Usamos 1/10 del tamaño de pantalla para minimizar uso de RAM
#define ILI9225_BUFFER_SIZE (ILI9225_WIDTH * ILI9225_HEIGHT / 10)

// Registros del ILI9225
#define ILI9225_DRIVER_OUTPUT_CTRL 0x01
#define ILI9225_LCD_AC_DRIVING_CTRL 0x02
#define ILI9225_ENTRY_MODE 0x03
#define ILI9225_DISP_CTRL1 0x07
#define ILI9225_BLANK_PERIOD_CTRL1 0x08
#define ILI9225_FRAME_CYCLE_CTRL 0x0B
#define ILI9225_INTERFACE_CTRL 0x0C
#define ILI9225_OSC_CTRL 0x0F
#define ILI9225_POWER_CTRL1 0x10
#define ILI9225_POWER_CTRL2 0x11
#define ILI9225_POWER_CTRL3 0x12
#define ILI9225_POWER_CTRL4 0x13
#define ILI9225_POWER_CTRL5 0x14
#define ILI9225_VCI_RECYCLING 0x15
#define ILI9225_RAM_ADDR_SET1 0x20
#define ILI9225_RAM_ADDR_SET2 0x21
#define ILI9225_GRAM_DATA_REG 0x22
#define ILI9225_GATE_SCAN_CTRL 0x30
#define ILI9225_VERTICAL_SCROLL_CTRL1 0x31
#define ILI9225_VERTICAL_SCROLL_CTRL2 0x32
#define ILI9225_VERTICAL_SCROLL_CTRL3 0x33
#define ILI9225_PARTIAL_DRIVING_POS1 0x34
#define ILI9225_PARTIAL_DRIVING_POS2 0x35
#define ILI9225_HORIZONTAL_WINDOW_ADDR1 0x36
#define ILI9225_HORIZONTAL_WINDOW_ADDR2 0x37
#define ILI9225_VERTICAL_WINDOW_ADDR1 0x38
#define ILI9225_VERTICAL_WINDOW_ADDR2 0x39
#define ILI9225_GAMMA_CTRL1 0x50
#define ILI9225_GAMMA_CTRL2 0x51
#define ILI9225_GAMMA_CTRL3 0x52
#define ILI9225_GAMMA_CTRL4 0x53
#define ILI9225_GAMMA_CTRL5 0x54
#define ILI9225_GAMMA_CTRL6 0x55
#define ILI9225_GAMMA_CTRL7 0x56
#define ILI9225_GAMMA_CTRL8 0x57
#define ILI9225_GAMMA_CTRL9 0x58
#define ILI9225_GAMMA_CTRL10 0x59

class ILI9225Display
{
  public:
    ILI9225Display(int8_t cs, int8_t dc, int8_t rst, int8_t mosi, int8_t sclk, int8_t bl);

    bool init();
    void flush(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p);
    void setBrightness(uint8_t brightness);
    uint8_t getBrightness() const;

    uint16_t getWidth() const { return ILI9225_WIDTH; }
    uint16_t getHeight() const { return ILI9225_HEIGHT; }

    lv_display_t *getLVGLDisplay() { return lvgl_display; }

  private:
    lv_display_t *lvgl_display;

    // Buffers pequeños para minimizar uso de RAM
    static lv_color_t buf1[ILI9225_BUFFER_SIZE];
    static lv_color_t buf2[ILI9225_BUFFER_SIZE];

    int8_t _cs, _dc, _rst, _mosi, _sclk, _bl;
    uint8_t _brightness;

    // Funciones de bajo nivel
    void writeCommand(uint8_t cmd);
    void writeData(uint8_t data);
    void writeData16(uint16_t data);
    void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void hardwareInit();

    static void flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p);
};
