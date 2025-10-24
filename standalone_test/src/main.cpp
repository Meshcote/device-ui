#include <Arduino.h>
#include <SPI.h>
#include <lvgl.h>

// Configuración para ILI9225 176x220
#define SCREEN_WIDTH 176
#define SCREEN_HEIGHT 220
#define BUFFER_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10)

// Pines del display
#define TFT_CS 7
#define TFT_DC 5
#define TFT_RST 6
#define TFT_MOSI 4
#define TFT_SCLK 3
#define TFT_BL 2

// Variables globales
static lv_display_t *disp;
static lv_color_t buf1[BUFFER_SIZE];
static lv_color_t buf2[BUFFER_SIZE];

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

// Declaraciones adelantadas
void ili9225_write_command(uint8_t cmd);
void ili9225_write_data(uint8_t data);
void ili9225_write_data16(uint16_t data);

// Función de inicialización completa del ILI9225
void ili9225_init()
{
    Serial.println("Inicializando ILI9225...");

    // Reset por hardware
    digitalWrite(TFT_RST, LOW);
    delay(50);
    digitalWrite(TFT_RST, HIGH);
    delay(150);

    // Inicialización según datasheet ILI9225
    ili9225_write_command(ILI9225_POWER_CTRL1);
    ili9225_write_data16(0x0000);
    ili9225_write_command(ILI9225_POWER_CTRL2);
    ili9225_write_data16(0x0000);
    ili9225_write_command(ILI9225_POWER_CTRL3);
    ili9225_write_data16(0x0000);
    ili9225_write_command(ILI9225_POWER_CTRL4);
    ili9225_write_data16(0x0000);
    ili9225_write_command(ILI9225_POWER_CTRL5);
    ili9225_write_data16(0x0000);
    delay(40);

    // Power-on sequence
    ili9225_write_command(ILI9225_POWER_CTRL2);
    ili9225_write_data16(0x0018);
    ili9225_write_command(ILI9225_POWER_CTRL3);
    ili9225_write_data16(0x6121);
    ili9225_write_command(ILI9225_POWER_CTRL4);
    ili9225_write_data16(0x006F);
    ili9225_write_command(ILI9225_POWER_CTRL5);
    ili9225_write_data16(0x495F);
    ili9225_write_command(ILI9225_POWER_CTRL1);
    ili9225_write_data16(0x0800);
    delay(10);
    ili9225_write_command(ILI9225_POWER_CTRL2);
    ili9225_write_data16(0x103B);
    delay(50);

    // Driver output control
    ili9225_write_command(ILI9225_DRIVER_OUTPUT_CTRL);
    ili9225_write_data16(0x011C);

    // LCD driving control
    ili9225_write_command(ILI9225_LCD_AC_DRIVING_CTRL);
    ili9225_write_data16(0x0100);

    // Entry mode
    ili9225_write_command(ILI9225_ENTRY_MODE);
    ili9225_write_data16(0x1030);

    // Display control
    ili9225_write_command(ILI9225_DISP_CTRL1);
    ili9225_write_data16(0x0000);

    // Blank period control
    ili9225_write_command(ILI9225_BLANK_PERIOD_CTRL1);
    ili9225_write_data16(0x0808);

    // Frame cycle control
    ili9225_write_command(ILI9225_FRAME_CYCLE_CTRL);
    ili9225_write_data16(0x1100);

    // Interface control
    ili9225_write_command(ILI9225_INTERFACE_CTRL);
    ili9225_write_data16(0x0000);

    // Oscillation control
    ili9225_write_command(ILI9225_OSC_CTRL);
    ili9225_write_data16(0x0D01);
    ili9225_write_command(ILI9225_VCI_RECYCLING);
    ili9225_write_data16(0x0020);

    // RAM address set
    ili9225_write_command(ILI9225_RAM_ADDR_SET1);
    ili9225_write_data16(0x0000);
    ili9225_write_command(ILI9225_RAM_ADDR_SET2);
    ili9225_write_data16(0x0000);

    // Gate scan control
    ili9225_write_command(ILI9225_GATE_SCAN_CTRL);
    ili9225_write_data16(0x0000);

    // Vertical scroll control
    ili9225_write_command(ILI9225_VERTICAL_SCROLL_CTRL1);
    ili9225_write_data16(0x00DB);
    ili9225_write_command(ILI9225_VERTICAL_SCROLL_CTRL2);
    ili9225_write_data16(0x0000);
    ili9225_write_command(ILI9225_VERTICAL_SCROLL_CTRL3);
    ili9225_write_data16(0x0000);

    // Partial driving position
    ili9225_write_command(ILI9225_PARTIAL_DRIVING_POS1);
    ili9225_write_data16(0x00DB);
    ili9225_write_command(ILI9225_PARTIAL_DRIVING_POS2);
    ili9225_write_data16(0x0000);

    // Horizontal and vertical RAM address position
    ili9225_write_command(ILI9225_HORIZONTAL_WINDOW_ADDR1);
    ili9225_write_data16(0x00AF);
    ili9225_write_command(ILI9225_HORIZONTAL_WINDOW_ADDR2);
    ili9225_write_data16(0x0000);
    ili9225_write_command(ILI9225_VERTICAL_WINDOW_ADDR1);
    ili9225_write_data16(0x00DB);
    ili9225_write_command(ILI9225_VERTICAL_WINDOW_ADDR2);
    ili9225_write_data16(0x0000);

    // Gamma control
    ili9225_write_command(ILI9225_GAMMA_CTRL1);
    ili9225_write_data16(0x0000);
    ili9225_write_command(ILI9225_GAMMA_CTRL2);
    ili9225_write_data16(0x0808);
    ili9225_write_command(ILI9225_GAMMA_CTRL3);
    ili9225_write_data16(0x080A);
    ili9225_write_command(ILI9225_GAMMA_CTRL4);
    ili9225_write_data16(0x000A);
    ili9225_write_command(ILI9225_GAMMA_CTRL5);
    ili9225_write_data16(0x0A08);
    ili9225_write_command(ILI9225_GAMMA_CTRL6);
    ili9225_write_data16(0x0808);
    ili9225_write_command(ILI9225_GAMMA_CTRL7);
    ili9225_write_data16(0x0000);
    ili9225_write_command(ILI9225_GAMMA_CTRL8);
    ili9225_write_data16(0x0A00);
    ili9225_write_command(ILI9225_GAMMA_CTRL9);
    ili9225_write_data16(0x0710);
    ili9225_write_command(ILI9225_GAMMA_CTRL10);
    ili9225_write_data16(0x0710);

    // Display on
    ili9225_write_command(ILI9225_DISP_CTRL1);
    ili9225_write_data16(0x0012);
    delay(50);
    ili9225_write_command(ILI9225_DISP_CTRL1);
    ili9225_write_data16(0x1017);

    Serial.println("ILI9225 inicializado completamente");
}

void ili9225_write_command(uint8_t cmd)
{
    digitalWrite(TFT_DC, LOW);
    digitalWrite(TFT_CS, LOW);
    SPI.transfer(0x00); // High byte (siempre 0 para comandos)
    SPI.transfer(cmd);  // Low byte (comando)
    digitalWrite(TFT_CS, HIGH);
}

void ili9225_write_data(uint8_t data)
{
    digitalWrite(TFT_DC, HIGH);
    digitalWrite(TFT_CS, LOW);
    SPI.transfer(data);
    digitalWrite(TFT_CS, HIGH);
}

void ili9225_write_data16(uint16_t data)
{
    digitalWrite(TFT_DC, HIGH);
    digitalWrite(TFT_CS, LOW);
    SPI.transfer(data >> 8);   // High byte primero
    SPI.transfer(data & 0xFF); // Low byte después
    digitalWrite(TFT_CS, HIGH);
}

void ili9225_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    ili9225_write_command(ILI9225_HORIZONTAL_WINDOW_ADDR1);
    ili9225_write_data16(x1);
    ili9225_write_command(ILI9225_HORIZONTAL_WINDOW_ADDR2);
    ili9225_write_data16(x0);

    ili9225_write_command(ILI9225_VERTICAL_WINDOW_ADDR1);
    ili9225_write_data16(y1);
    ili9225_write_command(ILI9225_VERTICAL_WINDOW_ADDR2);
    ili9225_write_data16(y0);

    ili9225_write_command(ILI9225_RAM_ADDR_SET1);
    ili9225_write_data16(x0);
    ili9225_write_command(ILI9225_RAM_ADDR_SET2);
    ili9225_write_data16(y0);

    ili9225_write_command(ILI9225_GRAM_DATA_REG);
}

// Callback de flush para LVGL
void my_disp_flush(lv_display_t *display, const lv_area_t *area, uint8_t *px_map)
{
    uint16_t width = area->x2 - area->x1 + 1;
    uint16_t height = area->y2 - area->y1 + 1;

    // Establecer ventana de escritura
    ili9225_set_window(area->x1, area->y1, area->x2, area->y2);

    // Transferir datos pixel por pixel
    uint16_t *pixels = (uint16_t *)px_map;
    uint32_t total_pixels = width * height;

    digitalWrite(TFT_DC, HIGH);
    digitalWrite(TFT_CS, LOW);

    for (uint32_t i = 0; i < total_pixels; i++) {
        SPI.write16(pixels[i]);
    }

    digitalWrite(TFT_CS, HIGH);

    lv_display_flush_ready(display);
}

// Control de brillo LED backlight
void setBacklight(uint8_t brightness)
{
    ledcWrite(0, brightness);
}

// Crear UI de prueba
void createTestUI()
{
    // Pantalla principal
    lv_obj_t *screen = lv_screen_active();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), 0);

    // Título
    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "MESHTASTIC");
    lv_obj_set_style_text_color(title, lv_color_hex(0x00FF00), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // Información del dispositivo
    lv_obj_t *device_info = lv_label_create(screen);
    lv_label_set_text(device_info, "Heltec V3\n"
                                   "ILI9225\n"
                                   "176x220\n"
                                   "Sin PSRAM");
    lv_obj_set_style_text_align(device_info, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(device_info, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(device_info, LV_ALIGN_CENTER, 0, -20);

    // Barra de estado
    lv_obj_t *bar = lv_bar_create(screen);
    lv_obj_set_size(bar, 140, 15);
    lv_bar_set_value(bar, 85, LV_ANIM_OFF);
    lv_obj_align(bar, LV_ALIGN_CENTER, 0, 40);

    // Memoria
    lv_obj_t *mem_label = lv_label_create(screen);
    lv_label_set_text_fmt(mem_label, "RAM: %d KB", ESP.getFreeHeap() / 1024);
    lv_obj_set_style_text_color(mem_label, lv_color_hex(0xFFFF00), 0);
    lv_obj_align(mem_label, LV_ALIGN_BOTTOM_MID, 0, -20);

    // Indicador PSRAM
    lv_obj_t *psram_label = lv_label_create(screen);
    lv_label_set_text(psram_label, "PSRAM: OFF");
    lv_obj_set_style_text_color(psram_label, lv_color_hex(0xFF0000), 0);
    lv_obj_align(psram_label, LV_ALIGN_BOTTOM_MID, 0, -5);
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n\n");
    Serial.println("╔════════════════════════════════════════╗");
    Serial.println("║  Heltec WiFi LoRa V3 + ILI9225 Test   ║");
    Serial.println("║           Sin PSRAM                    ║");
    Serial.println("╚════════════════════════════════════════╝");
    Serial.println();

    // Información del sistema
    Serial.printf("Chip: %s\n", ESP.getChipModel());
    Serial.printf("CPU: %d MHz\n", ESP.getCpuFreqMHz());
    Serial.printf("Flash: %d KB\n", ESP.getFlashChipSize() / 1024);
    Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("PSRAM: %d bytes\n\n", ESP.getPsramSize());

    // Configurar pines
    pinMode(TFT_CS, OUTPUT);
    pinMode(TFT_DC, OUTPUT);
    pinMode(TFT_RST, OUTPUT);
    pinMode(TFT_BL, OUTPUT);

    digitalWrite(TFT_CS, HIGH);
    digitalWrite(TFT_DC, HIGH);
    digitalWrite(TFT_RST, HIGH);

    Serial.println("Configuración de pines:");
    Serial.printf("  CS  (Chip Select): GPIO %d\n", TFT_CS);
    Serial.printf("  DC  (Data/Cmd):    GPIO %d\n", TFT_DC);
    Serial.printf("  RST (Reset):       GPIO %d\n", TFT_RST);
    Serial.printf("  MOSI (Data):       GPIO %d\n", TFT_MOSI);
    Serial.printf("  SCLK (Clock):      GPIO %d\n", TFT_SCLK);
    Serial.printf("  BL  (Backlight):   GPIO %d\n\n", TFT_BL);

    // Configurar backlight
    Serial.println("Configurando backlight...");
    ledcSetup(0, 5000, 8);
    ledcAttachPin(TFT_BL, 0);
    setBacklight(255);
    Serial.println("Backlight ON");

    // Inicializar SPI
    Serial.println("Inicializando SPI...");
    SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
    SPI.setFrequency(10000000); // 10MHz
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    Serial.println("SPI OK");

    // Inicializar display
    ili9225_init();

    // Test visual extendido
    Serial.println("\n=== TEST VISUAL ===");

    // Test 1: Pantalla roja completa
    Serial.println("Test 1: Llenando pantalla de ROJO...");
    ili9225_set_window(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    digitalWrite(TFT_DC, HIGH);
    digitalWrite(TFT_CS, LOW);
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        SPI.transfer(0xF8); // R=11111
        SPI.transfer(0x00); // G=000000, B=00000
    }
    digitalWrite(TFT_CS, HIGH);
    Serial.println("-> Espera 2 segundos (debería verse ROJO)");
    delay(2000);

    // Test 2: Pantalla verde
    Serial.println("Test 2: Llenando pantalla de VERDE...");
    ili9225_set_window(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    digitalWrite(TFT_DC, HIGH);
    digitalWrite(TFT_CS, LOW);
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        SPI.transfer(0x07); // R=00000
        SPI.transfer(0xE0); // G=111111, B=00000
    }
    digitalWrite(TFT_CS, HIGH);
    Serial.println("-> Espera 2 segundos (debería verse VERDE)");
    delay(2000);

    // Test 3: Pantalla azul
    Serial.println("Test 3: Llenando pantalla de AZUL...");
    ili9225_set_window(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    digitalWrite(TFT_DC, HIGH);
    digitalWrite(TFT_CS, LOW);
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        SPI.transfer(0x00); // R=00000
        SPI.transfer(0x1F); // G=000000, B=11111
    }
    digitalWrite(TFT_CS, HIGH);
    Serial.println("-> Espera 2 segundos (debería verse AZUL)");
    delay(2000);

    // Test 4: Patrón de franjas
    Serial.println("Test 4: Dibujando franjas de colores...");
    ili9225_set_window(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    digitalWrite(TFT_DC, HIGH);
    digitalWrite(TFT_CS, LOW);
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        int row = i / SCREEN_WIDTH;
        if (row < 55) {
            SPI.transfer(0xF8);
            SPI.transfer(0x00); // Rojo
        } else if (row < 110) {
            SPI.transfer(0x07);
            SPI.transfer(0xE0); // Verde
        } else if (row < 165) {
            SPI.transfer(0x00);
            SPI.transfer(0x1F); // Azul
        } else {
            SPI.transfer(0xFF);
            SPI.transfer(0xFF); // Blanco
        }
    }
    digitalWrite(TFT_CS, HIGH);
    Serial.println("-> Debería verse 4 franjas: ROJO, VERDE, AZUL, BLANCO");
    Serial.println("\n=== FIN TESTS VISUALES ===\n");
    delay(3000);

    // Inicializar LVGL
    Serial.println("\nInicializando LVGL...");
    lv_init();

    disp = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_display_set_buffers(disp, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(disp, my_disp_flush);

    Serial.printf("Buffer size: %d bytes x2\n", sizeof(buf1));
    Serial.printf("Total buffer: %d KB\n", (sizeof(buf1) + sizeof(buf2)) / 1024);

    // Crear UI
    createTestUI();

    Serial.println("\n=== Sistema listo ===");
    Serial.printf("Free heap: %d bytes\n\n", ESP.getFreeHeap());
}

void loop()
{
    lv_timer_handler();
    delay(5);

    // Info cada segundo
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 1000) {
        lastPrint = millis();
        Serial.printf("[%lu] Heap: %d\n", millis() / 1000, ESP.getFreeHeap());
    }
}
