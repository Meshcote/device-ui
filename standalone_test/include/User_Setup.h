// Configuración personalizada TFT_eSPI para ILI9225
// Este archivo debe estar en el directorio include/

#pragma once

#define USER_SETUP_LOADED

// Driver
#define ILI9225_DRIVER

// Pines (se definen en platformio.ini pero también aquí por compatibilidad)
#ifndef TFT_CS
#define TFT_CS 7
#endif

#ifndef TFT_DC
#define TFT_DC 5
#endif

#ifndef TFT_RST
#define TFT_RST 6
#endif

#ifndef TFT_MOSI
#define TFT_MOSI 4
#endif

#ifndef TFT_SCLK
#define TFT_SCLK 3
#endif

#ifndef TFT_BL
#define TFT_BL 2
#endif

// Frecuencias SPI
#define SPI_FREQUENCY 27000000
#define SPI_READ_FREQUENCY 16000000

// Fuentes
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_GFXFF

#define SMOOTH_FONT
