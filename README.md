# Smart Roots Connect

> **A robust microcontroller integration layer for Smart Roots: bridging sensor hardware, actuators, and the cloud ecosystem.**

---

## 📜 Project Overview

Smart Roots Connect acts as the middleware/firmware glue for the Smart Roots ecosystem, enabling any hydroponic tent or environmental control hardware (usually based on ESP32 or similar platforms) to reliably connect with the Smart Roots Server and App. Its purpose is to give growers and developers a powerful but flexible firmware architecture that carries data (from sensors) and commands (to relays, pumps, etc.) between the cloud and physical world.

---

## ✨ Features (Based on Project Context)

- **Sensor Aggregation & Polling**
  - Periodically reads sensor data from multiple types (EC, pH, temperature, humidity, water level, and more).
  - Supports averaging/debouncing for better measurement reliability.

- **Actuator/Relay Control**
  - Controls and toggles a variety of actuators including pumps, lights, extractors, dosing pumps, and fans.
  - GPIO/relay mapping in code, supporting multiple channels/devices.

- **Flexible Connectivity**
  - WiFi-first communication (compatible with ESP32, Arduino-style platforms).
  - Sends data either via MQTT (preferred) to a broker such as EMQX/Smart Roots Server, or HTTP REST as backup.

- **Device Identification**
  - Every node identified by its MAC address or similar unique ID, matching it to a tent in the Smart Roots Server/App.

- **OTA Update Ready**
  - Can receive firmware updates “over the air,” reducing manual maintenance.

- **Onboard Status & Diagnostics**
  - Supports displays such as I2C OLED or LCD for quick health/readout.
  - Serial debug output, colored LED error/status indicators.

- **Robust Protocol Implementation**
  - MQTT for both telemetry (`Readings/{mac}`) and remote actuation (`Toggle/{mac}`).
  - Topics/JSON schemas designed to perfectly mirror what Smart Roots Server and App expect.

- **Network Setup & Provisioning**
  - WiFiManager (or similar captive portal system) for quick, in-field network configuration.

---

## 🏗️ Core Architecture

- **Loop-based firmware:** Main loop cycles through sensor reads, publishes values, checks for incoming actuation commands, and refreshes display/status.
- **Interrupt-safe:** Debouncing, critical section protection for liquid dosing/fan/pump controls.
- **Configurable Pinout:** All sensors and actuators are mapped via code constants, supporting many PCB layouts or breadboard rigs.

---

## 🔗 Integration Points

- **Smart Roots Server:** Bidirectional link—publishes telemetry for cloud storage/analytics, subscribes to actuator commands.
- **Smart Roots App:** Reflects live status and allows the user direct, instant remote control over connected devices.
- **Other Devices:** Multiple tents/devices can coexist, each addressable by MAC.

---

## 🛠 Supported Hardware (Typical/Intended)

- MCU: ESP32 (Arduino IDE or PlatformIO)
- Sensors: EC, pH, temperature, humidity, light, water level (digital and analog variants)
- Actuators: Relays, dosing pumps, fans, lights
- Displays: OLED I2C, serial LCD

---

## 🧩 Room for Expansion

(as per code conventions and context)
- Add support for BLE/Ethernet/LoRa comms.
- Onboard web-based settings/configuration portal.
- More advanced fault/alert logic (run autonomously if cloud is unreachable).
- Modular hardware abstraction for third-party sensor/actuator support.
- Edge analytics or automated local control routines.

---

## 🚦 Getting Started

1. Flash firmware using PlatformIO/Arduino IDE to your ESP32-based tent device.
2. Configure WiFi, MQTT/HTTP endpoints, MAC/device ID, and pinout in code before flashing.
3. Power up and provision network/wifi; connect to server and verify App control.
4. Calibrate sensors, test relays, check MQTT/HTTP logs for end-to-end connectivity.

---

## 🏷️ License

See the repo for current licensing information.

---

> *Connects your crops, cloud, and control—built by Code-Syndicate-SH.*
