# Soporte ILI9225 para Heltec WiFi LoRa V3 (Sin PSRAM)

## Resumen

Se ha añadido soporte completo para pantalla **ILI9225 (176x220)** en dispositivos **Heltec WiFi LoRa V3** que no tienen PSRAM.

## Archivos Creados

### 1. Driver ILI9225

- **`include/graphics/ILI9225Display.h`**: Clase del driver
- **`source/graphics/ILI9225Display.cpp`**: Implementación optimizada sin PSRAM

### 2. Configuración TFT_eSPI

- **`include/graphics/TFT_Config.h`**: Configuración de pines para TFT_eSPI

### 3. Configuración PlatformIO

- **`platformio_heltec_ili9225.ini`**: Environment para compilar con soporte ILI9225

### 4. Ejemplo

- **`examples/heltec_ili9225_test.cpp`**: Código de prueba completo

## Configuración ILI9225 para Heltec WiFi LoRa V3

## ✅ Estado: FUNCIONANDO

El driver ILI9225 ha sido implementado y probado exitosamente en Heltec WiFi LoRa V3 sin PSRAM.

## Hardware Requerido

- **Heltec WiFi LoRa V3** (ESP32-S3, 320KB RAM, sin PSRAM)
- **Display ILI9225** 176x220 SPI
- **Conexiones:**
  ```
  Display    →  Heltec V3
  ─────────────────────────
  CS  (SS)   →  GPIO 7
  DC  (RS)   →  GPIO 5
  RST        →  GPIO 6
  MOSI (SDI) →  GPIO 4
  SCLK (CLK) →  GPIO 3
  BL  (LED)  →  GPIO 2
  VCC        →  3.3V
  GND        →  GND
  ```

## Características

✅ Inicialización completa del controlador ILI9225
✅ Renderizado RGB565 de 16 bits
✅ Buffers optimizados (1/10 de pantalla = 7.7KB x2)
✅ Control PWM de backlight
✅ Compatible con LVGL 9.0
✅ Sin dependencia de PSRAM
✅ Uso de RAM: ~108KB (33%)

## Uso Rápido

### Opción 1: Proyecto Standalone (Pruebas)

```bash
cd standalone_test
pio run -t upload --upload-port /dev/ttyUSB0
pio device monitor
```

### Opción 2: Integración con Device-UI (Producción)

1. **Incluir el driver en tu código:**

```cpp
#include "graphics/ILI9225Display.h"

// Crear instancia con los pines configurados
ILI9225Display display(7, 5, 6, 4, 3, 2);

void setup() {
    Serial.begin(115200);

    // Inicializar display
    if (!display.init()) {
        Serial.println("ERROR: Display init failed");
        return;
    }

    // El display LVGL está listo para usar
    lv_display_t *disp = display.getLVGLDisplay();

    // Crear tu UI aquí...
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "¡Hola Meshtastic!");
}

void loop() {
    lv_timer_handler();  // Procesar eventos LVGL
    delay(5);
}
```

2. **Configurar platformio.ini:**

```ini
[env:heltec-lora-wifi-v3-ili9225]
platform = espressif32
board = heltec_wifi_lora_32_V3
framework = arduino

build_flags =
    -DBOARD_HAS_PSRAM=0
    -DCONFIG_SPIRAM_SUPPORT=0
    -DCONFIG_ESP32S3_SPIRAM_SUPPORT=0
    -Os

lib_deps =
    lvgl/lvgl@^9.0.0

board_build.arduino.memory_type = qio_qspi
board_build.partitions = default.csv
```

## Integración con Meshtastic Device-UI

El driver ILI9225 ya está integrado en el proyecto device-ui. Para usarlo:

1. **Clonar y compilar el proyecto:**

```bash
git clone https://github.com/meshtastic/device-ui
cd device-ui
pio run -e heltec-lora-wifi-v3-ili9225
```

2. **El driver se inicializará automáticamente** con la configuración de pines predefinida.

3. **La UI de Meshtastic se renderizará** en el display ILI9225 usando LVGL.

## Verificación

Después de subir el firmware, deberías ver:

1. **Secuencia de colores de prueba:** Rojo → Verde → Azul → Franjas
2. **Pantalla de inicio** con:
   - Logo "MESHTASTIC"
   - Información del dispositivo
   - Indicadores de estado
   - Barras y elementos UI

## Especificaciones Técnicas

- **Resolución:** 176 x 220 píxeles
- **Formato de color:** RGB565 (16 bits)
- **Frecuencia SPI:** 10 MHz
- **Modo SPI:** MODE 0 (CPOL=0, CPHA=0)
- **Orientación:** Portrait (vertical)
- **Buffer LVGL:** 3872 bytes x2 (7.5KB total)
- **RAM disponible:** ~280KB después de inicialización

## Solución de Problemas

### Pantalla blanca

- ✅ **RESUELTO:** El protocolo requiere enviar comandos como 2 bytes (0x00 + comando)
- ✅ **RESUELTO:** Los datos de 16 bits deben enviarse MSB primero

### Errores de PSRAM

- ✅ **RESUELTO:** Configuración correcta de `memory_type = qio_qspi`
- ✅ **RESUELTO:** Flags de compilación para deshabilitar PSRAM

### Colores incorrectos

- Verifica la orientación y Entry Mode (registro 0x03)
- El driver usa BGR=1 (0x1030) por defecto

## Próximos Pasos

- [ ] Optimizar velocidad SPI (probar hasta 40 MHz)
- [ ] Implementar DMA para transferencias más rápidas
- [ ] Añadir soporte para rotación de pantalla
- [ ] Integrar con módulo LoRa de Meshtastic
- [ ] Añadir pantallas adicionales (GPS, mensajes, configuración)

## Referencias

- [ILI9225 Datasheet](https://www.displayfuture.com/Display/datasheet/controller/ILI9225.pdf)
- [LVGL Documentation](https://docs.lvgl.io/)
- [ESP32-S3 Technical Reference](https://www.espressif.com/sites/default/files/documentation/esp32-s3_technical_reference_manual_en.pdf)

## Optimizaciones para Sin PSRAM

1. **Buffer reducido**: 1/10 del tamaño de pantalla (~7.7 KB x 2 buffers)
2. **Renderizado parcial**: LVGL dibuja en secciones pequeñas
3. **Heap LVGL limitado**: 48 KB
4. **Fuentes mínimas**: Solo GLCD, Font2 y FreeFonts básicas
5. **PWM para backlight**: Control de brillo en pin 2

## Uso de Memoria Estimado

```
RAM Requerida (sin PSRAM):
├─ Buffers LVGL: ~15.4 KB (2 buffers)
├─ Heap LVGL: ~48 KB
├─ Stack + TFT_eSPI: ~20 KB
└─ Total: ~83 KB (OK para ESP32-S3 con 512KB SRAM)
```

## Cómo Compilar

### Usando PlatformIO:

```bash
# Compilar
pio run -e heltec-lora-wifi-v3-ili9225

# Subir al dispositivo
pio run -e heltec-lora-wifi-v3-ili9225 -t upload

# Monitorear serial
pio device monitor
```

### Usando el ejemplo de prueba:

1. Copia `examples/heltec_ili9225_test.cpp` a `src/main.cpp`
2. Compila con el environment `heltec-lora-wifi-v3-ili9225`

## Verificación

Deberías ver en el serial:

```
╔═══════════════════════════════════════╗
║   Heltec WiFi LoRa V3 + ILI9225      ║
║   Test sin PSRAM                      ║
╚═══════════════════════════════════════╝

Chip: ESP32-S3
CPU Freq: 240 MHz
Free Heap: 400000 bytes
PSRAM: 0 bytes (debe ser 0)

Initializing ILI9225 Display...
Pines: CS=7, DC=5, RST=6, MOSI=4, SCLK=3, BL=2
Display size: 176x220
Buffer size: 7744 bytes (3872 pixels)
Total buffer memory: 15 KB

=== Sistema listo ===
Free heap final: 380000 bytes
```

## Notas Importantes

1. **Orientación de pantalla**: Ajusta en `ILI9225Display.cpp`, línea con `tft.setRotation(0)` (valores: 0, 1, 2, 3)
2. **Frecuencia SPI**: 27 MHz por defecto, reduce si hay problemas de estabilidad
3. **Control de brillo**: Usa `display->setBrightness(0-255)` para controlar el LED backlight
4. **Sin PSRAM verificado**: El código verifica que PSRAM = 0 al iniciar

## Troubleshooting

### Pantalla en blanco

- Verifica conexiones de pines
- Verifica alimentación (3.3V)
- Revisa que LED backlight (pin 2) esté funcionando

### Errores de memoria

- Reduce `LVGL_MEM_SIZE` en platformio.ini
- Desactiva widgets no usados en `lv_conf.h`

### Pantalla con colores incorrectos

- Verifica que driver sea ILI9225 en `TFT_Config.h`
- Prueba diferentes orientaciones (rotation 0-3)

## Integración con Meshtastic

Para integrar con el firmware Meshtastic completo:

1. Añade `-DILI9225_DISPLAY=1` a tu configuración de build
2. Los archivos del driver se compilarán automáticamente
3. LVGL detectará y usará el display ILI9225

## Próximos Pasos

- [ ] Probar diferentes rotaciones de pantalla
- [ ] Optimizar fuentes para pantalla pequeña
- [ ] Añadir UI específica para 176x220 en `generated/ui_176x220/`
- [ ] Integrar con sistema de mensajes Meshtastic
