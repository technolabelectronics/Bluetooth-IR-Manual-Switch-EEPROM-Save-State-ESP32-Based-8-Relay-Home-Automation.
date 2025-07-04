/*
 * ================================================================================
 * 8-Relay Home Automation System Using ESP32 | Bluetooth, IR Remote & Manual Control
 * ================================================================================
 * 
 * This code allows you to control 8 electrical appliances using:
 *   ✅ Bluetooth commands
 *   ✅ IR remote signals
 *   ✅ Physical switches (with press/release detection)
 * 
 * Relay states are saved in EEPROM so that after a power cut, the system restores the previous state.
 * 
 * -------------------------------------------------------------------------------
 * 📌 Developed by Asheesh Maurya
 * 💼 Technolab Electronics | www.technolabelectronics.com
 * 📧 support@technolabelectronics.com
 * 📍 Based in Prayagraj, Uttar Pradesh, India
 * 
 * © 2024 Technolab Electronics. All rights reserved.
 * This code is licensed for educational and commercial use with proper credit.
 * -------------------------------------------------------------------------------
 */

#include "BluetoothSerial.h"   // For Bluetooth communication using ESP32
#include <AceButton.h>         // For smart button press/release detection
#include <IRremote.h>          // To receive and decode IR remote signals
#include <EEPROM.h>            // To store relay state persistently in memory

using namespace ace_button;   // Allows use of AceButton classes directly without prefix

BluetoothSerial SerialBT;     // Create a Bluetooth object named SerialBT

// -----------------------------
// Define IR Receiver Pin
// -----------------------------
#define IR_RECEIVE_PIN 12     // GPIO12 connected to IR receiver

// -----------------------------
// Relay Pin Definitions
// -----------------------------
#define RelayPin1 23
#define RelayPin2 22
#define RelayPin3 21
#define RelayPin4 19
#define RelayPin5 18
#define RelayPin6 5
#define RelayPin7 17
#define RelayPin8 4

// -----------------------------
// Switch Pin Definitions (Push Buttons)
// -----------------------------
#define SwitchPin1 34
#define SwitchPin2 35
#define SwitchPin3 32
#define SwitchPin4 33
#define SwitchPin5 25
#define SwitchPin6 26
#define SwitchPin7 27
#define SwitchPin8 14

// -----------------------------
// EEPROM and State Variables
// -----------------------------
#define EEPROM_SIZE 8                    // 1 byte for each relay (total 8)
unsigned long lastIRTime = 0;           // Last time IR command was received
const unsigned long IR_DEBOUNCE_DELAY = 300;  // Delay to avoid double toggling

int toggleState[8] = {1, 1, 1, 1, 1, 1, 1, 1}; // Initial relay state: 1 = OFF (HIGH)
char bt_data;                                // Holds Bluetooth command

// -----------------------------
// Button Configurations for Physical Switches
// -----------------------------
ButtonConfig config[8];                      // Config for each AceButton
AceButton button[] = {
  AceButton(&config[0]), AceButton(&config[1]), AceButton(&config[2]), AceButton(&config[3]),
  AceButton(&config[4]), AceButton(&config[5]), AceButton(&config[6]), AceButton(&config[7])
};

// -----------------------------
// Relay and Switch Pin Arrays (used in loops)
// -----------------------------
const uint8_t relayPins[8] = {RelayPin1, RelayPin2, RelayPin3, RelayPin4, RelayPin5, RelayPin6, RelayPin7, RelayPin8};
const uint8_t switchPins[8] = {SwitchPin1, SwitchPin2, SwitchPin3, SwitchPin4, SwitchPin5, SwitchPin6, SwitchPin7, SwitchPin8};

// -----------------------------
// IR Remote Codes (You can change these based on your remote)
// -----------------------------
#define IR_CODE_R1_TOGGLE  0xC00002
#define IR_CODE_R2_TOGGLE  0xC00001
#define IR_CODE_R3_TOGGLE  0xC00003
#define IR_CODE_R4_TOGGLE  0xC00004
#define IR_CODE_R5_TOGGLE  0xC00005
#define IR_CODE_R6_TOGGLE  0xC00006
#define IR_CODE_R7_TOGGLE  0xC00007
#define IR_CODE_R8_TOGGLE  0xC00008
#define IR_CODE_ALL_ON     0xC00009
#define IR_CODE_ALL_OFF    0xC000010

// Function to turn all relays ON and update EEPROM
void all_Switch_ON() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(relayPins[i], LOW);      // Active LOW relays
    toggleState[i] = 0;
    EEPROM.write(i, 0);
  }
  EEPROM.commit();  // Save changes to EEPROM
}

// Function to turn all relays OFF and update EEPROM
void all_Switch_OFF() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(relayPins[i], HIGH);     // Relay OFF
    toggleState[i] = 1;
    EEPROM.write(i, 1);
  }
  EEPROM.commit();
}

// Function to handle incoming Bluetooth commands
void Bluetooth_handle() {
  bt_data = SerialBT.read();   // Read Bluetooth character
  delay(20);                   // Small delay for stability

  switch (bt_data) {
    case 'a': digitalWrite(RelayPin1, LOW); toggleState[0] = 0; EEPROM.write(0, 0); break;
    case 'A': digitalWrite(RelayPin1, HIGH); toggleState[0] = 1; EEPROM.write(0, 1); break;
    case 'b': digitalWrite(RelayPin2, LOW); toggleState[1] = 0; EEPROM.write(1, 0); break;
    case 'B': digitalWrite(RelayPin2, HIGH); toggleState[1] = 1; EEPROM.write(1, 1); break;
    case 'c': digitalWrite(RelayPin3, LOW); toggleState[2] = 0; EEPROM.write(2, 0); break;
    case 'C': digitalWrite(RelayPin3, HIGH); toggleState[2] = 1; EEPROM.write(2, 1); break;
    case 'd': digitalWrite(RelayPin4, LOW); toggleState[3] = 0; EEPROM.write(3, 0); break;
    case 'D': digitalWrite(RelayPin4, HIGH); toggleState[3] = 1; EEPROM.write(3, 1); break;
    case 'e': digitalWrite(RelayPin5, LOW); toggleState[4] = 0; EEPROM.write(4, 0); break;
    case 'E': digitalWrite(RelayPin5, HIGH); toggleState[4] = 1; EEPROM.write(4, 1); break;
    case 'f': digitalWrite(RelayPin6, LOW); toggleState[5] = 0; EEPROM.write(5, 0); break;
    case 'F': digitalWrite(RelayPin6, HIGH); toggleState[5] = 1; EEPROM.write(5, 1); break;
    case 'g': digitalWrite(RelayPin7, LOW); toggleState[6] = 0; EEPROM.write(6, 0); break;
    case 'G': digitalWrite(RelayPin7, HIGH); toggleState[6] = 1; EEPROM.write(6, 1); break;
    case 'h': digitalWrite(RelayPin8, LOW); toggleState[7] = 0; EEPROM.write(7, 0); break;
    case 'H': digitalWrite(RelayPin8, HIGH); toggleState[7] = 1; EEPROM.write(7, 1); break;
    case '1': all_Switch_ON(); break;
    case '2': all_Switch_OFF(); break;
    default: return;
  }
  EEPROM.commit();
}

// Function to handle IR remote signals
void IR_handle() {
  if (IrReceiver.decode()) {
    unsigned long currentTime = millis();
    if (currentTime - lastIRTime < IR_DEBOUNCE_DELAY) {
      IrReceiver.resume();
      return;
    }
    lastIRTime = currentTime;

    uint32_t code = IrReceiver.decodedIRData.decodedRawData;

    if (code == IR_CODE_R1_TOGGLE) { toggleState[0] = !toggleState[0]; digitalWrite(relayPins[0], toggleState[0]); EEPROM.write(0, toggleState[0]); SerialBT.print(toggleState[0] ? 'A' : 'a'); }
    else if (code == IR_CODE_R2_TOGGLE) { toggleState[1] = !toggleState[1]; digitalWrite(relayPins[1], toggleState[1]); EEPROM.write(1, toggleState[1]); SerialBT.print(toggleState[1] ? 'B' : 'b'); }
    else if (code == IR_CODE_R3_TOGGLE) { toggleState[2] = !toggleState[2]; digitalWrite(relayPins[2], toggleState[2]); EEPROM.write(2, toggleState[2]); SerialBT.print(toggleState[2] ? 'C' : 'c'); }
    else if (code == IR_CODE_R4_TOGGLE) { toggleState[3] = !toggleState[3]; digitalWrite(relayPins[3], toggleState[3]); EEPROM.write(3, toggleState[3]); SerialBT.print(toggleState[3] ? 'D' : 'd'); }
    else if (code == IR_CODE_R5_TOGGLE) { toggleState[4] = !toggleState[4]; digitalWrite(relayPins[4], toggleState[4]); EEPROM.write(4, toggleState[4]); SerialBT.print(toggleState[4] ? 'E' : 'e'); }
    else if (code == IR_CODE_R6_TOGGLE) { toggleState[5] = !toggleState[5]; digitalWrite(relayPins[5], toggleState[5]); EEPROM.write(5, toggleState[5]); SerialBT.print(toggleState[5] ? 'F' : 'f'); }
    else if (code == IR_CODE_R7_TOGGLE) { toggleState[6] = !toggleState[6]; digitalWrite(relayPins[6], toggleState[6]); EEPROM.write(6, toggleState[6]); SerialBT.print(toggleState[6] ? 'G' : 'g'); }
    else if (code == IR_CODE_R8_TOGGLE) { toggleState[7] = !toggleState[7]; digitalWrite(relayPins[7], toggleState[7]); EEPROM.write(7, toggleState[7]); SerialBT.print(toggleState[7] ? 'H' : 'h'); }
    else if (code == IR_CODE_ALL_ON) { all_Switch_ON(); SerialBT.print('1'); }
    else if (code == IR_CODE_ALL_OFF) { all_Switch_OFF(); SerialBT.print('2'); }

    EEPROM.commit();
    IrReceiver.resume();
  }
}

// Setup function runs once when ESP32 starts
void setup() {
  Serial.begin(9600);
  btStart();                            // Start Bluetooth controller
  SerialBT.begin("HA_BT_ESP32");        // Set Bluetooth device name
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start IR receiver
  EEPROM.begin(EEPROM_SIZE);           // Initialize EEPROM memory

  for (int i = 0; i < 8; i++) {
    toggleState[i] = EEPROM.read(i);                // Load previous relay state
    pinMode(relayPins[i], OUTPUT);                  // Set relay pin as output
    pinMode(switchPins[i], INPUT_PULLUP);           // Set switch pin as input with pull-up
    digitalWrite(relayPins[i], toggleState[i]);     // Set relay state
    config[i].setEventHandler(handleButtonEvent);   // Assign button event handler
    button[i].init(switchPins[i]);                  // Initialize button
  }
  delay(500);  // Allow time for system to stabilize
}

// Main loop continuously runs after setup
void loop() {
  if (SerialBT.available()) Bluetooth_handle();  // Handle Bluetooth commands
  IR_handle();                                   // Handle IR commands
  for (int i = 0; i < 8; i++) button[i].check(); // Check all button states
}

// Event handler for AceButton: handles press/release events
void handleButtonEvent(AceButton* btn, uint8_t eventType, uint8_t buttonState) {
  int index = btn - button;  // Determine which button triggered the event
  if (eventType == AceButton::kEventPressed) {
    toggleState[index] = 0;
    digitalWrite(relayPins[index], LOW);         // Turn relay ON
    EEPROM.write(index, 0);
    SerialBT.print("abcdefgh"[index]);           // Feedback to app
  } else if (eventType == AceButton::kEventReleased) {
    toggleState[index] = 1;
    digitalWrite(relayPins[index], HIGH);        // Turn relay OFF
    EEPROM.write(index, 1);
    SerialBT.print("ABCDEFGH"[index]);           // Feedback to app
  }
  EEPROM.commit();
}
