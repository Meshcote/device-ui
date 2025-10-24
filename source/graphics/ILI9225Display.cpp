#include "graphics/ILI9225Display.h"
#include <Arduino.h>
#include <SPI.h>

// Definición de buffers estáticos
lv_color_t ILI9225Display::buf1[ILI9225_BUFFER_SIZE];
lv_color_t ILI9225Display::buf2[ILI9225_BUFFER_SIZE];

// Instancia estática para el callback
static ILI9225Display *instance = nullptr;

ILI9225Display::ILI9225Display(int8_t cs, int8_t dc, int8_t rst, int8_t mosi, int8_t sclk, int8_t bl)
    : _cs(cs), _dc(dc), _rst(rst), _mosi(mosi), _sclk(sclk), _bl(bl), _brightness(255)
{
    instance = this;
}

bool ILI9225Display::init()
{
    Serial.println("[ILI9225] Inicializando display...");

    // Configurar pines
    pinMode(_cs, OUTPUT);
    pinMode(_dc, OUTPUT);
    pinMode(_rst, OUTPUT);
    pinMode(_bl, OUTPUT);

    digitalWrite(_cs, HIGH);
    digitalWrite(_dc, HIGH);
    digitalWrite(_rst, HIGH);

    // Configurar backlight PWM
    ledcSetup(0, 5000, 8);
    ledcAttachPin(_bl, 0);
    setBrightness(_brightness);

    // Inicializar SPI
    SPI.begin(_sclk, -1, _mosi, _cs);
    SPI.setFrequency(10000000); // 10MHz
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);

    // Inicializar hardware ILI9225
    hardwareInit();

    // Inicializar LVGL
    lv_init();

    lvgl_display = lv_display_create(ILI9225_WIDTH, ILI9225_HEIGHT);
    lv_display_set_buffers(lvgl_display, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(lvgl_display, flush_cb);

    // SIN rotación - portrait normal

    Serial.printf("[ILI9225] Display inicializado: %dx%d\n", ILI9225_WIDTH, ILI9225_HEIGHT);
    Serial.printf("[ILI9225] Buffer: %d bytes x2 = %d KB\n", ILI9225_BUFFER_SIZE * sizeof(lv_color_t),
                  (ILI9225_BUFFER_SIZE * sizeof(lv_color_t) * 2) / 1024);

    return true;
}

void ILI9225Display::hardwareInit()
{
    Serial.println("[ILI9225] Inicializando controlador...");

    // Reset por hardware
    digitalWrite(_rst, LOW);
    delay(50);
    digitalWrite(_rst, HIGH);
    delay(150);

    // Inicialización según datasheet ILI9225
    writeCommand(ILI9225_POWER_CTRL1);
    writeData16(0x0000);
    writeCommand(ILI9225_POWER_CTRL2);
    writeData16(0x0000);
    writeCommand(ILI9225_POWER_CTRL3);
    writeData16(0x0000);
    writeCommand(ILI9225_POWER_CTRL4);
    writeData16(0x0000);
    writeCommand(ILI9225_POWER_CTRL5);
    writeData16(0x0000);
    delay(40);

    // Power-on sequence
    writeCommand(ILI9225_POWER_CTRL2);
    writeData16(0x0018);
    writeCommand(ILI9225_POWER_CTRL3);
    writeData16(0x6121);
    writeCommand(ILI9225_POWER_CTRL4);
    writeData16(0x006F);
    writeCommand(ILI9225_POWER_CTRL5);
    writeData16(0x495F);
    writeCommand(ILI9225_POWER_CTRL1);
    writeData16(0x0800);
    delay(10);
    writeCommand(ILI9225_POWER_CTRL2);
    writeData16(0x103B);
    delay(50);

    // Driver output control - Portrait normal
    writeCommand(ILI9225_DRIVER_OUTPUT_CTRL);
    writeData16(0x011C); // Configuración original

    // LCD driving control
    writeCommand(ILI9225_LCD_AC_DRIVING_CTRL);
    writeData16(0x0100);

    // Entry mode - Portrait normal
    writeCommand(ILI9225_ENTRY_MODE);
    writeData16(0x1030); // Configuración original

    // Display control
    writeCommand(ILI9225_DISP_CTRL1);
    writeData16(0x0000);

    // Blank period control
    writeCommand(ILI9225_BLANK_PERIOD_CTRL1);
    writeData16(0x0808);

    // Frame cycle control
    writeCommand(ILI9225_FRAME_CYCLE_CTRL);
    writeData16(0x1100);

    // Interface control
    writeCommand(ILI9225_INTERFACE_CTRL);
    writeData16(0x0000);

    // Oscillation control
    writeCommand(ILI9225_OSC_CTRL);
    writeData16(0x0D01);
    writeCommand(ILI9225_VCI_RECYCLING);
    writeData16(0x0020);

    // RAM address set
    writeCommand(ILI9225_RAM_ADDR_SET1);
    writeData16(0x0000);
    writeCommand(ILI9225_RAM_ADDR_SET2);
    writeData16(0x0000);

    // Gate scan control
    writeCommand(ILI9225_GATE_SCAN_CTRL);
    writeData16(0x0000);

    // Vertical scroll control
    writeCommand(ILI9225_VERTICAL_SCROLL_CTRL1);
    writeData16(0x00DB);
    writeCommand(ILI9225_VERTICAL_SCROLL_CTRL2);
    writeData16(0x0000);
    writeCommand(ILI9225_VERTICAL_SCROLL_CTRL3);
    writeData16(0x0000);

    // Partial driving position
    writeCommand(ILI9225_PARTIAL_DRIVING_POS1);
    writeData16(0x00DB);
    writeCommand(ILI9225_PARTIAL_DRIVING_POS2);
    writeData16(0x0000);

    // Horizontal and vertical RAM address position
    writeCommand(ILI9225_HORIZONTAL_WINDOW_ADDR1);
    writeData16(0x00AF);
    writeCommand(ILI9225_HORIZONTAL_WINDOW_ADDR2);
    writeData16(0x0000);
    writeCommand(ILI9225_VERTICAL_WINDOW_ADDR1);
    writeData16(0x00DB);
    writeCommand(ILI9225_VERTICAL_WINDOW_ADDR2);
    writeData16(0x0000);

    // Gamma control
    writeCommand(ILI9225_GAMMA_CTRL1);
    writeData16(0x0000);
    writeCommand(ILI9225_GAMMA_CTRL2);
    writeData16(0x0808);
    writeCommand(ILI9225_GAMMA_CTRL3);
    writeData16(0x080A);
    writeCommand(ILI9225_GAMMA_CTRL4);
    writeData16(0x000A);
    writeCommand(ILI9225_GAMMA_CTRL5);
    writeData16(0x0A08);
    writeCommand(ILI9225_GAMMA_CTRL6);
    writeData16(0x0808);
    writeCommand(ILI9225_GAMMA_CTRL7);
    writeData16(0x0000);
    writeCommand(ILI9225_GAMMA_CTRL8);
    writeData16(0x0A00);
    writeCommand(ILI9225_GAMMA_CTRL9);
    writeData16(0x0710);
    writeCommand(ILI9225_GAMMA_CTRL10);
    writeData16(0x0710);

    // Display on
    writeCommand(ILI9225_DISP_CTRL1);
    writeData16(0x0012);
    delay(50);
    writeCommand(ILI9225_DISP_CTRL1);
    writeData16(0x1017);

    Serial.println("[ILI9225] Controlador inicializado");
}

void ILI9225Display::writeCommand(uint8_t cmd)
{
    digitalWrite(_dc, LOW);
    digitalWrite(_cs, LOW);
    SPI.transfer(0x00); // High byte
    SPI.transfer(cmd);  // Low byte
    digitalWrite(_cs, HIGH);
}

void ILI9225Display::writeData(uint8_t data)
{
    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);
    SPI.transfer(data);
    digitalWrite(_cs, HIGH);
}

void ILI9225Display::writeData16(uint16_t data)
{
    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);
    SPI.transfer(data >> 8);   // High byte
    SPI.transfer(data & 0xFF); // Low byte
    digitalWrite(_cs, HIGH);
}

void ILI9225Display::setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    writeCommand(ILI9225_HORIZONTAL_WINDOW_ADDR1);
    writeData16(x1);
    writeCommand(ILI9225_HORIZONTAL_WINDOW_ADDR2);
    writeData16(x0);

    writeCommand(ILI9225_VERTICAL_WINDOW_ADDR1);
    writeData16(y1);
    writeCommand(ILI9225_VERTICAL_WINDOW_ADDR2);
    writeData16(y0);

    writeCommand(ILI9225_RAM_ADDR_SET1);
    writeData16(x0);
    writeCommand(ILI9225_RAM_ADDR_SET2);
    writeData16(y0);

    writeCommand(ILI9225_GRAM_DATA_REG);
}

void ILI9225Display::flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    uint16_t width = area->x2 - area->x1 + 1;
    uint16_t height = area->y2 - area->y1 + 1;

    // Establecer ventana de escritura
    setWindow(area->x1, area->y1, area->x2, area->y2);

    // Transferir datos pixel por pixel
    uint16_t *pixels = (uint16_t *)px_map;
    uint32_t total_pixels = width * height;

    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);

    for (uint32_t i = 0; i < total_pixels; i++) {
        uint16_t color = pixels[i];
        SPI.transfer(color >> 8);
        SPI.transfer(color & 0xFF);
    }

    digitalWrite(_cs, HIGH);

    lv_display_flush_ready(disp);
}

void ILI9225Display::flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p)
{
    if (instance) {
        instance->flush(disp, area, color_p);
    }
}

void ILI9225Display::setBrightness(uint8_t brightness)
{
    _brightness = brightness;
    ledcWrite(0, brightness);
}

uint8_t ILI9225Display::getBrightness() const
{
    return _brightness;
}
