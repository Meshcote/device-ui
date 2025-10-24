#pragma once

// ============================================
// Configuración TFT_eSPI para ILI9225
// Heltec WiFi LoRa V3 + ILI9225 176x220
// ============================================

// Driver ILI9225
#define ILI9225_DRIVER

// Pines de pantalla ILI9225 (según hardware del usuario)
#define TFT_CS 7   // Chip Select
#define TFT_DC 5   // Data/Command (RS)
#define TFT_RST 6  // Reset
#define TFT_MOSI 4 // SDI (MOSI)
#define TFT_SCLK 3 // CL (Clock)
// TFT_MISO no se usa en ILI9225 (solo escritura)

// Pin de retroiluminación LED
#define TFT_BL 2 // Backlight LED
#define TFT_BACKLIGHT_ON HIGH

// Pines SD Card (opcional)
#define SD_CS 1
#define SD_MOSI 38
#define SD_MISO 39
#define SD_SCK 40

// Frecuencia SPI optimizada para ESP32-S3 sin PSRAM
#define SPI_FREQUENCY 27000000 // 27MHz
#define SPI_READ_FREQUENCY 16000000

// Fuentes mínimas para ahorrar memoria
#define LOAD_GLCD  // Font 1. Original Adafruit 8 pixel font
#define LOAD_FONT2 // Font 2. Small 16 pixel high font
#define LOAD_GFXFF // FreeFonts

// Sin touch screen
#define TOUCH_CS -1

// Smooth fonts
#define SMOOTH_FONT
