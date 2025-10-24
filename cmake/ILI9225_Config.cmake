# Configuración CMake para ILI9225 en Heltec WiFi LoRa V3
# Sin PSRAM

set(ILI9225_DISPLAY ON CACHE BOOL "Enable ILI9225 display support")
set(BOARD_HAS_PSRAM OFF CACHE BOOL "Board does not have PSRAM")

# Pines de hardware
add_compile_definitions(
    ILI9225_DISPLAY=1
    BOARD_HAS_PSRAM=0
    TFT_CS=7
    TFT_DC=5
    TFT_RST=6
    TFT_MOSI=4
    TFT_SCLK=3
    TFT_BL=2
    SCREEN_WIDTH=176
    SCREEN_HEIGHT=220
)

# Optimizaciones de memoria
add_compile_options(-Os)
add_compile_definitions(
    LVGL_MEM_SIZE=49152
    LV_CONF_INCLUDE_SIMPLE
)

message(STATUS "ILI9225 Display: Enabled")
message(STATUS "PSRAM: Disabled")
message(STATUS "Display Size: 176x220")
message(STATUS "Pins: CS=7, DC=5, RST=6, MOSI=4, SCLK=3, BL=2")
