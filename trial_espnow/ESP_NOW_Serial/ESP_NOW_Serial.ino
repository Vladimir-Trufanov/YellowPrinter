/*
    Последовательный пример ESP-NOW - одноадресная передача данных
    Lucas Saavedra Vaz - 2024
    
    Передача данных между двумя ESP32 с использованием протокола ESP-NOW в конфигурации "один к одному" (unicast).
    Обратите внимание, что для разных интерфейсов используются разные MAC-адреса.
    Устройства могут работать в разных режимах (точка доступа или станция), но при этом поддерживать связь с помощью ESP-NOW.
    Единственное требование заключается в том, чтобы устройства подключались к одному и тому же каналу Wi-Fi.
    Установите MAC-адрес узла в соответствии с устройством, которое будет принимать данные.

    Пример настройки:
    
    - Устройство 1: Режим точки доступа с MAC-адресом F6:12:FA:42:B6:E8
                MAC-адрес узла устанавливается на MAC-адрес станции устройства 2 (F4:12:FA:40:64:4C)
                
    - Устройство 2: Режим станции с MAC-адресом F4:12:FA:40:64:4C.
                MAC-адрес однорангового устройства, установленный на MAC-адрес точки доступа устройства 1 (F6:12:FA:42:B6:E8)

    Устройство, на котором запущен этот sketch, также будет получать и печатать данные с любого устройства, MAC-адрес которого установлен в качестве MAC-адреса однорангового устройства.
    Чтобы правильно визуализировать передаваемые данные, установите для конца строки в последовательном мониторе значение "Как NL, так и CR".
*/

#include <Arduino.h>
#include "ESP32_NOW_Serial.h"
#include "MacAddress.h"
#include "WiFi.h"

#include "esp_wifi.h"

// 0: AP mode, 1: Station mode
#define ESPNOW_WIFI_MODE_STATION 1

// Channel to be used by the ESP-NOW protocol
#define ESPNOW_WIFI_CHANNEL 1

#if ESPNOW_WIFI_MODE_STATION          // ESP-NOW using WiFi Station mode
#define ESPNOW_WIFI_MODE WIFI_STA     // WiFi Mode
#define ESPNOW_WIFI_IF   WIFI_IF_STA  // WiFi Interface
#else                                 // ESP-NOW using WiFi AP mode
#define ESPNOW_WIFI_MODE WIFI_AP      // WiFi Mode
#define ESPNOW_WIFI_IF   WIFI_IF_AP   // WiFi Interface
#endif

/*
 AI Thinker ESP32-CAM [6] - MAC Address: F8:B3:B7:A7:E1:18
 ESP32-2432S028 (CYD) [7] - MAC Address: B8:D6:1A:5C:F9:54
*/

// Set the MAC address of the device that will receive the data
// For example: F4:12:FA:40:64:4C
// const MacAddress peer_mac({0xF4, 0x12, 0xFA, 0x40, 0x64, 0x4C});
const MacAddress peer_mac({0xB8, 0xD6, 0x1A, 0x5C, 0xF9, 0x54}); // AI Thinker ESP32-CAM 6

ESP_NOW_Serial_Class NowSerial(peer_mac, ESPNOW_WIFI_CHANNEL, ESPNOW_WIFI_IF);

void setup() {
  Serial.begin(115200);

  Serial.print("WiFi Mode: ");
  Serial.println(ESPNOW_WIFI_MODE == WIFI_AP ? "AP" : "Station");
  WiFi.mode(ESPNOW_WIFI_MODE);

  Serial.print("Channel: ");
  Serial.println(ESPNOW_WIFI_CHANNEL);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);

  while (!(WiFi.STA.started() || WiFi.AP.started())) {
    delay(100);
  }

  Serial.print("MAC Address: ");
  Serial.println(ESPNOW_WIFI_MODE == WIFI_AP ? WiFi.softAPmacAddress() : WiFi.macAddress());

  // Start the ESP-NOW communication
  Serial.println("ESP-NOW communication starting...");
  NowSerial.begin(115200);
  Serial.printf("ESP-NOW version: %d, max data length: %d\n", ESP_NOW.getVersion(), ESP_NOW.getMaxDataLen());
  Serial.println("You can now send data to the peer device using the Serial Monitor.\n");
}

void loop() {
  while (NowSerial.available()) {
    Serial.write(NowSerial.read());
  }

  while (Serial.available() && NowSerial.availableForWrite()) {
    if (NowSerial.write(Serial.read()) <= 0) {
      Serial.println("Failed to send data");
      break;
    }
  }

  delay(1);
}
