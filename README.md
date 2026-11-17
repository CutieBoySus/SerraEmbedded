# SerraEmbedded — Smart Automated Greenhouse

**SerraEmbedded** is an automated indoor smart greenhouse system built around an **ESP32-S3-CAM** microcontroller. It monitors soil moisture across multiple independent pots, air temperature/humidity, and water tank levels in real time, automating irrigation (3 independent 12V pumps) and anti-condensation ventilation via a local asynchronous web dashboard with live MJPEG camera streaming.

---

## 1. Hardware & Software Requirements

### Hardware
* **Main Microcontroller:** ESP32-S3-CAM (N16R8 — 16MB Flash, 8MB Octal PSRAM) with OV2640/OV3660 camera module.
* **Analog Expansion:** ADS1115 16-bit 4-channel I2C ADC (for capacitive soil moisture sensors 1–4) + 2 native `ADC1` channels (`GPIO 1`, `GPIO 2`, Wi-Fi safe).
* **Environmental & Level Sensors:**
  * 6× Capacitive Soil Moisture Sensors v1.2 (analog output).
  * 1× DHT22 digital temperature and humidity sensor.
  * 1× HC-SR04 ultrasonic distance sensor (water tank level monitoring, with $1\text{k}\Omega / 2\text{k}\Omega$ voltage divider on `ECHO`).
* **Actuators & Power Stage:**
  * 4-Channel MOSFET Driver Module (low-side switching for 12V inductive/DC loads).
  * 3× 12V Submersible Micro Water Pumps + drip irrigation tubing.
  * 1× 12V DC Axial Fan (PWM speed control for air circulation and anti-condensation).
  * 1× 5V LED Grow Strip.
  * Power Supply: 12V 3A DC adapter + DC-DC Step-Down Buck Converter ($12\text{V} \rightarrow 5.0\text{V}$).
* **Enclosure:** Custom acrylic greenhouse structure + 3D-printed splash-proof electronics housing.

### Software
* **VS Code + PlatformIO IDE:** For building and flashing the C++ firmware (`espressif32` platform, Arduino framework).
* **Libraries (`platformio.ini`):** `Adafruit ADS1X15`, `DHT sensor library`, `ArduinoJson`, `ESPAsyncWebServer`, `AsyncTCP`, `esp32-camera`.
* **Web Dashboard:** Vanilla HTML5, CSS3, and JavaScript served directly from the ESP32-S3 LittleFS filesystem.

---

## 2. Wiring & Pinout

### ESP32-S3-CAM Connections (provvisorio magari lo faremo grafico)

| Peripheral | Module Pin | ESP32-S3 Pin | Notes |
| :--- | :--- | :--- | :--- |
| **Soil Sensor 1** | `AOUT` | `GPIO 1` | Native `ADC1_CH0` (active with Wi-Fi) |
| **Soil Sensor 2** | `AOUT` | `GPIO 2` | Native `ADC1_CH1` (active with Wi-Fi) |
| **ADS1115 ADC** | `SDA` / `SCL` | `GPIO 48` / `GPIO 47` | I2C Bus (`ADDR` tied to `GND` $\rightarrow$ `0x48`) |
| **DHT22 Sensor** | `DATA` | `GPIO 21` | Powered at 3.3V |
| **HC-SR04 Ultrasonic** | `TRIG` | `GPIO 45` | Digital Output (3.3V) |
| **HC-SR04 Ultrasonic** | `ECHO` | `GPIO 38` | **5V $\rightarrow$ 3.3V voltage divider required** |
| **Pump 1 (MOSFET Ch1)** | `IN1` | `GPIO 41` | Digital / PWM control |
| **Pump 2 (MOSFET Ch2)** | `IN2` | `GPIO 42` | Digital / PWM control |
| **Pump 3 (MOSFET Ch3)** | `IN3` | `GPIO 40` | Digital / PWM control |
| **12V Fan (MOSFET Ch4)** | `IN4` | `GPIO 39` | PWM speed modulation |

### Power Rails Summary (provvisorio magari lo faremo grafico)

| Rail | Source | Powers |
| :--- | :--- | :--- |
| **12V DC** | 12V 3A Wall Adapter | MOSFET `VIN` (3× Water Pumps, 12V Fan), Step-Down input |
| **5.0V DC** | Step-Down Buck Converter | ESP32-S3 `5V` pin, HC-SR04 `VCC`, 5V LED Strip |
| **3.3V DC** | ESP32-S3 On-board Regulator | ADS1115 `VDD`, DHT22 `VCC`, Capacitive Soil Sensors `VCC` |
| **GND** | Common Ground | **Must be shared across 12V PSU, Step-Down, MOSFETs, and ESP32** |

---

## 3. Project Layout

```text
SerraEmbedded/
├── firmware/                       # PlatformIO project for ESP32-S3-CAM
│   ├── platformio.ini              # Board config (16MB Flash, 8MB OPI PSRAM) & dependencies
│   ├── include/                    # Header files (.h)
│   │   ├── Config.h                # GPIO definitions, calibration values, and safety thresholds
│   │   ├── SystemState.h           # Shared state struct across tasks and web server
│   │   ├── soil_sensors.h          # ADS1115 + native ADC1 driver interface
│   │   ├── climate_tank.h          # DHT22 + HC-SR04 driver interface
│   │   ├── actuators.h             # Non-blocking MOSFET controller & fail-safe logic
│   │   └── web_server.h            # Async HTTP server, REST API & MJPEG camera stream
│   ├── src/                        # C++ implementations (.cpp)
│   │   ├── main.cpp                # System initialization & FreeRTOS dual-core task scheduling
│   │   ├── soil_sensors.cpp        # Moisture filtering & hysteresis logic
│   │   ├── climate_tank.cpp        # Ultrasonic tank level & anti-condensation fan logic
│   │   ├── actuators.cpp           # Timed pump state machine & dry-run protection
│   │   └── web_server.cpp          # JSON telemetry endpoints & camera frame handler
│   └── data/                       # Static web frontend (uploaded to LittleFS)
│       ├── index.html              # Responsive monitoring & control dashboard
│       ├── style.css               # Dashboard styling
│       └── app.js                  # Asynchronous status polling & manual override controls
├── hardware/                       # Electrical schematics & mechanical files
│   ├── schematic.png               # Full circuit diagram
│   └── cad/                        # 3D-printable STL files for electronics enclosure
├── images/                         # Hardware photos & wiring documentation
├── .gitignore
└── README.md
```

---

## 4. How to Build, Flash, and Run

1. **Clone the repository:**
   ```bash
   git clone [https://github.com/CutieBoySus/SerraEmbedded.git](https://github.com/CutieBoySus/SerraEmbedded.git)
   cd SerraEmbedded/firmware
   ```
2. **Configure Wi-Fi Credentials:**
   Create `firmware/include/Secrets.h` (ignored by Git) with your network details:
   ```cpp
   #pragma once
   #define WIFI_SSID "Your_WiFi_SSID"
   #define WIFI_PASS "Your_WiFi_Password"
   ```
3. **Build and Upload Firmware:**
   Connect the ESP32-S3 via USB and run via PlatformIO CLI (or use the VS Code PlatformIO sidebar):
   ```bash
   pio run -t upload
   ```
4. **Upload Web Dashboard Filesystem (LittleFS):**
   ```bash
   pio run -t uploadfs
   ```
5. **Access the Dashboard:**
   Open the Serial Monitor (`115200 baud`) to read the assigned IP address, then open in your browser:
   * **Dashboard UI:** `http://<ESP32_IP>/`
   * **Live Camera Stream:** `http://<ESP32_IP>/stream`

---

## 5. Media & Documentation

* **Presentation Slides:** questo lo aggiugneremo
* **Demo Video:** questo lo aggiugneremo

---

## 6. Team Members & Contributions (DI ESEMPIO RAGA)

* **Alexandro Aydagulov (Alex):** System architecture, FreeRTOS task orchestration, MOSFET actuator state machine & fail-safes, Async Web Server, MJPEG camera integration, and Web Dashboard UI.
* **Mattia Degiampietro:** Soil moisture subsystem, I2C ADS1115 + native ADC1 driver implementation, wet/dry sensor calibration, and irrigation hysteresis logic.
* **Raja Faizan Ali:** Environmental & water level subsystem, DHT22 climate driver, HC-SR04 ultrasonic tank calculation, and dry-run safety lockout.
* **Taufge Songne:** Complete electrical schematic, power rail management (12V/5V/3.3V), 3D CAD electronics enclosure, acrylic greenhouse assembly, and drip irrigation plumbing.