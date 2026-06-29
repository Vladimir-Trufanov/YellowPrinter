/*
    ESP-NOW Broadcast Slave
    Lucas Saavedra Vaz - 2024

    name=ESP_NOW
    version=3.3.10
    author=me-no-dev
    maintainer=P-R-O-C-H-Y
    sentence=Library for ESP_NOW
    paragraph=Supports ESP32 Arduino platforms.
    category=Communication
    url=https://github.com/espressif/arduino-esp32/
    architectures=esp32

    В этом наброске показано, как получать широковещательные сообщения от ведущего устройства с использованием протокола ESP-NOW.

    Ведущее устройство будет передавать сообщение каждые 5 секунд на все устройства в сети.

    Подчиненные устройства будут получать широковещательные сообщения. Если они не от известного ведущего, они будут зарегистрированы как новые ведущие
    с использованием функции обратного вызова.
*/

/*
   AI-Thinker ESP32-CAM
   Arduino 1.8.18
   ESP32 3.3.8
*/

/* 
 Диалог, снятый 2026-06-29, при первом запуске:
  
ESP-NOW Example - Broadcast Slave
Wi-Fi parameters:
  Mode: STA
  MAC Address: F8:B3:B7:A7:E1:18
  Channel: 6
ESP-NOW version: 2, max data length: 1470
Setup complete. Waiting for a master to broadcast a message...
Unknown peer b8:d6:1a:5c:f9:54 sent a broadcast message
Registering the peer as a master
Successfully registered master b8:d6:1a:5c:f9:54 (total masters: 1)
Received a message from master b8:d6:1a:5c:f9:54 (broadcast)
  Message: Hello, World! #137
Registered masters: 1
  Master 0: b8:d6:1a:5c:f9:54
Received a message from master b8:d6:1a:5c:f9:54 (broadcast)
  Message: Hello, World! #138
Received a message from master b8:d6:1a:5c:f9:54 (broadcast)
  Message: Hello, World! #139
Registered masters: 1
  Master 0: b8:d6:1a:5c:f9:54
Received a message from master b8:d6:1a:5c:f9:54 (broadcast)
  Message: Hello, World! #140
Received a message from master b8:d6:1a:5c:f9:54 (broadcast)
  Message: Hello, World! #141
Registered masters: 1
  Master 0: b8:d6:1a:5c:f9:54
Received a message from master b8:d6:1a:5c:f9:54 (broadcast)
  Message: Hello, World! #142
Received a message from master b8:d6:1a:5c:f9:54 (broadcast)
  Message: Hello, World! #143
Registered masters: 1
  Master 0: b8:d6:1a:5c:f9:54
Received a message from master b8:d6:1a:5c:f9:54 (broadcast)
  Message: Hello, World! #144
Received a message from master b8:d6:1a:5c:f9:54 (broadcast)
  Message: Hello, World! #145
Registered masters: 1
  Master 0: b8:d6:1a:5c:f9:54
Received a message from master b8:d6:1a:5c:f9:54 (broadcast)
  Message: Hello, World! #146
  
*/

#include <Arduino.h>
#include "ESP32_NOW.h"
#include "WiFi.h"

#include <esp_mac.h>  // For the MAC2STR and MACSTR macros

#include <vector>

/* Definitions */

#define ESPNOW_WIFI_CHANNEL 6

/* Classes */

// Creating a new class that inherits from the ESP_NOW_Peer class is required.

class ESP_NOW_Peer_Class : public ESP_NOW_Peer {
public:
  // Constructor of the class
  ESP_NOW_Peer_Class(const uint8_t *mac_addr, uint8_t channel, wifi_interface_t iface, const uint8_t *lmk) : ESP_NOW_Peer(mac_addr, channel, iface, lmk) {}

  // Destructor of the class
  ~ESP_NOW_Peer_Class() {}

  // Function to register the master peer
  bool add_peer() {
    if (!add()) {
      log_e("Failed to register the broadcast peer");
      return false;
    }
    return true;
  }

  // Function to print the received messages from the master
  void onReceive(const uint8_t *data, size_t len, bool broadcast) {
    Serial.printf("Received a message from master " MACSTR " (%s)\n", MAC2STR(addr()), broadcast ? "broadcast" : "unicast");
    Serial.printf("  Message: %s\n", (char *)data);
  }
};

/* Global Variables */

// List of all the masters. It will be populated when a new master is registered
// Note: Using pointers instead of objects to prevent dangling pointers when the vector reallocates
std::vector<ESP_NOW_Peer_Class *> masters;

/* Callbacks */

// Callback called when an unknown peer sends a message
void register_new_master(const esp_now_recv_info_t *info, const uint8_t *data, int len, void *arg) {
  if (memcmp(info->des_addr, ESP_NOW.BROADCAST_ADDR, 6) == 0) {
    Serial.printf("Unknown peer " MACSTR " sent a broadcast message\n", MAC2STR(info->src_addr));
    Serial.println("Registering the peer as a master");

    ESP_NOW_Peer_Class *new_master = new ESP_NOW_Peer_Class(info->src_addr, ESPNOW_WIFI_CHANNEL, WIFI_IF_STA, nullptr);
    if (!new_master->add_peer()) {
      Serial.println("Failed to register the new master");
      delete new_master;
      return;
    }
    masters.push_back(new_master);
    Serial.printf("Successfully registered master " MACSTR " (total masters: %lu)\n", MAC2STR(new_master->addr()), (unsigned long)masters.size());
  } else {
    // The slave will only receive broadcast messages
    log_v("Received a unicast message from " MACSTR, MAC2STR(info->src_addr));
    log_v("Igorning the message");
  }
}

/* Main */

void setup() {
  Serial.begin(115200);

  // Initialize the Wi-Fi module
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) {
    delay(100);
  }

  Serial.println("ESP-NOW Example - Broadcast Slave");
  Serial.println("Wi-Fi parameters:");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %u\n", ESPNOW_WIFI_CHANNEL);

  // Initialize the ESP-NOW protocol
  if (!ESP_NOW.begin()) {
    Serial.println("Failed to initialize ESP-NOW");
    Serial.println("Reeboting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }

  Serial.printf("ESP-NOW version: %d, max data length: %d\n", ESP_NOW.getVersion(), ESP_NOW.getMaxDataLen());

  // Register the new peer callback
  ESP_NOW.onNewPeer(register_new_master, nullptr);

  Serial.println("Setup complete. Waiting for a master to broadcast a message...");
}

void loop() {
  // Print debug information every 10 seconds
  static unsigned long last_debug = 0;
  if (millis() - last_debug > 10000) {
    last_debug = millis();
    Serial.printf("Registered masters: %lu\n", (unsigned long)masters.size());
    for (size_t i = 0; i < masters.size(); i++) {
      if (masters[i]) {
        Serial.printf("  Master %lu: " MACSTR "\n", (unsigned long)i, MAC2STR(masters[i]->addr()));
      }
    }
  }

  delay(100);
}
