# Proyecto de prueba standalone para ILI9225 + Heltec WiFi LoRa V3

Este directorio contiene un proyecto Arduino/PlatformIO standalone para probar el display ILI9225.

## Estructura del proyecto

```
standalone_test/
├── platformio.ini       # Configuración PlatformIO
├── src/
│   └── main.cpp        # Código principal de prueba
├── include/
│   └── README.md
└── lib/
    └── README.md
```

## Instalación

1. **Instalar PlatformIO**:

   ```bash
   pip install platformio
   ```

2. **Compilar**:

   ```bash
   cd standalone_test
   pio run
   ```

3. **Subir al dispositivo**:

   ```bash
   pio run -t upload
   ```

4. **Monitorear serial**:
   ```bash
   pio device monitor
   ```

## Pines de conexión

```
ILI9225 → Heltec V3
├─ CS   → GPIO 7
├─ DC   → GPIO 5
├─ RST  → GPIO 6
├─ MOSI → GPIO 4
├─ SCLK → GPIO 3
├─ BL   → GPIO 2
└─ VCC  → 3.3V
└─ GND  → GND
```

## Notas

- Este es un proyecto de prueba simplificado
- No incluye todas las características de Meshtastic
- Sirve para verificar que el display funciona correctamente
