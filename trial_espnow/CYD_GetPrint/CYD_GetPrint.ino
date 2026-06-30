/*
  CYD_GetPrint.ino

  Принять сообщение по ESP-NOW и вывести его на экран фонтом по умолчанию
  
  На примере одноадресной передачи данных от Lucas Saavedra Vaz - 2024 ESP_NOW_Serial.ino
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

// Устанавливаем MAC-адрес устройства c которого будут поступать данные
const MacAddress peer_mac({0xF8, 0xB3, 0xB7, 0xA7, 0xE1, 0x18}); // AI Thinker ESP32-CAM 6
// Определяем объект для приема данных по ESP_NOW
ESP_NOW_Serial_Class NowSerial(peer_mac, ESPNOW_WIFI_CHANNEL, ESPNOW_WIFI_IF);

void setup() 
{
  Serial.begin(115200);
  // По умолчанию определяем режим станции
  Serial.print("Режим WiFi: ");
  Serial.println(ESPNOW_WIFI_MODE == WIFI_AP ? "собственный WiFi" : "как станция");
  WiFi.mode(ESPNOW_WIFI_MODE);

  Serial.print("Канал: ");
  Serial.println(ESPNOW_WIFI_CHANNEL);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);

  while (!(WiFi.STA.started() || WiFi.AP.started())) 
  {
    delay(100);
  }

  Serial.print("MAC адрес CYD: ");
  Serial.println(ESPNOW_WIFI_MODE == WIFI_AP ? WiFi.softAPmacAddress() : WiFi.macAddress());

  // Start the ESP-NOW communication
  Serial.println("ESP-NOW communication starting...");
  NowSerial.begin(115200);
  Serial.printf("ESP-NOW version: %d, max data length: %d\n", ESP_NOW.getVersion(), ESP_NOW.getMaxDataLen());
  Serial.println("Теперь можно вводить данные из последовательного порта и передавать по ESP_NOW,");
  Serial.println("а также выводить полученные данные в последовательный порт и на экран CYD.\n");
}

void loop() 
{
  while (NowSerial.available()) 
  {
    Serial.write(NowSerial.read());
  }

  while (Serial.available() && NowSerial.availableForWrite()) 
  {
    if (NowSerial.write(Serial.read()) <= 0) 
    {
      Serial.println("Ошибка отправки данных по ESP_NOW");
      break;
    }
  }
  // Делаем паузу на очистку мусора FreeRTOS
  delay(1);
}
