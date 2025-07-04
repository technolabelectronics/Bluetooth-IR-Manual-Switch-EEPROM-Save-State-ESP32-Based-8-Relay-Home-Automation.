# Bluetooth-IR-Manual-Switch-EEPROM-Save-State-ESP32-Based-8-Relay-Home-Automation.
# ESP32-Based 8-Relay Smart Home Automation System

Control your home appliances using an ESP32 with **Bluetooth**, **IR remote**, and **physical switches** – all in one!  
This project enables smart and flexible control over 8 relays, with built-in EEPROM memory to retain the last state even after a power loss.

![ESP32 Smart Relay Banner](https://your-image-link-if-any)

---

## 🔧 Features

- ✅ Control 8 relays (lights, fans, etc.)
- ✅ Bluetooth Classic control (via mobile app)
- ✅ IR remote control (custom or TV remote)
- ✅ Physical push buttons (press/release detection)
- ✅ EEPROM memory for saving relay states
- ✅ Fully commented and beginner-friendly Arduino code
- ✅ PCB designed using [EasyEDA](https://easyeda.com) – Free online PCB design tool

---

## 🧰 Hardware Used

- ESP32 Dev Board
- 8-Channel Relay Module
- IR Receiver (TSOP1838 or similar)
- Push Buttons (x8)
- Resistors for pull-up (optional, if not using INPUT_PULLUP)
- 5V Power Supply
- Custom PCB (optional but recommended)

---

## 🔌 Pin Configuration

| Relay        | ESP32 Pin |
|--------------|-----------|
| Relay 1      | GPIO 23   |
| Relay 2      | GPIO 22   |
| Relay 3      | GPIO 21   |
| Relay 4      | GPIO 19   |
| Relay 5      | GPIO 18   |
| Relay 6      | GPIO 5    |
| Relay 7      | GPIO 17   |
| Relay 8      | GPIO 4    |

| Switch       | ESP32 Pin |
|--------------|-----------|
| Switch 1     | GPIO 34   |
| Switch 2     | GPIO 35   |
| Switch 3     | GPIO 32   |
| Switch 4     | GPIO 33   |
| Switch 5     | GPIO 25   |
| Switch 6     | GPIO 26   |
| Switch 7     | GPIO 27   |
| Switch 8     | GPIO 14   |

IR Receiver: GPIO 12  
EEPROM Size: 8 bytes (1 byte per relay)

---

## 📲 Bluetooth Commands

| Command | Action        |
|---------|---------------|
| `a`     | Turn ON Relay 1 |
| `A`     | Turn OFF Relay 1 |
| `b`     | Turn ON Relay 2 |
| `B`     | Turn OFF Relay 2 |
| ...     | ...             |
| `1`     | Turn ON All Relays |
| `2`     | Turn OFF All Relays |

---

## 📺 IR Remote Codes

You can customize the IR codes in the code block:
```cpp
#define IR_CODE_R1_TOGGLE  0xC00002
...
#define IR_CODE_ALL_OFF    0xC000010

