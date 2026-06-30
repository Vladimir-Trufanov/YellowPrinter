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

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
#include <TFT_eSPI_Scroll.h>
#include <4bit.h>
TFT_eSPI_Scroll scroll;

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
 AI Thinker ESP32-CAM [8] - MAC Address: 94:54:C5:A8:21:9C
*/

// Устанавливаем MAC-адрес устройства c которого будут поступать данные
const MacAddress peer_mac({0xF8, 0xB3, 0xB7, 0xA7, 0xE1, 0x18}); // AI Thinker ESP32-CAM 6
// const MacAddress peer_mac({0x94, 0x54, 0xC5, 0xA8, 0x21, 0x9C}); // AI Thinker ESP32-CAM 8
// Определяем объект для приема данных по ESP_NOW
ESP_NOW_Serial_Class NowSerial(peer_mac, ESPNOW_WIFI_CHANNEL, ESPNOW_WIFI_IF);

void setup() 
{
  delay(3000);

  /*
  tft.init();                   // инициализация дисплея
  tft.setRotation(1);           // альбомная
  tft.fillScreen(TFT_BLACK);    // заливка фона чёрным цветом
  tft.setTextColor(TFT_WHITE);  // цвет текста - белый
  tft.setCursor(0,0);           // x,y координаты текста
  tft.setTextSize(2);           // размер текста - №2 (7*2 = 14 точек)
  tft.print("Привет!");       // вывод текста 
  tft.print("И пока.");         // вывод текста 
  */
 
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // Initializing the tft_espi_scroll int 1bit B/W
  if(scroll.init(&tft, 4) != NO_ERROR)
  {
    Serial.println("Failed... Reseting...");        
    return;
  }


  // инициализация SPIFFS
  if (!SPIFFS.begin()) 
  {
    while (1) yield();
  }  

  //tft.loadFont("nasalization48"); // загрузка в память шрифта
  //tft.loadFont("HuaweiSans16");     // загрузка в память шрифта
  tft.setCursor(0,0);
  tft.setTextSize(1);           // размер текста - №1
  //tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  //tft.println("ВНИМАНИЕ!");
  //tft.unloadFont();                 // выгрузка шрифта из памяти

  Serial.begin(115200);
  Serial.println("\n");

  // По умолчанию определяем режим станции
  say("Режим WiFi: ");
  sayln(ESPNOW_WIFI_MODE == WIFI_AP ? "собственный WiFi" : "как станция");
  WiFi.mode(ESPNOW_WIFI_MODE);

  say("Канал: ");
  sayln(ESPNOW_WIFI_CHANNEL);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);

  while (!(WiFi.STA.started() || WiFi.AP.started())) 
  {
    delay(100);
  }

  say("MAC адрес CYD: ");
  sayln(ESPNOW_WIFI_MODE == WIFI_AP ? WiFi.softAPmacAddress() : WiFi.macAddress());

  // Start the ESP-NOW communication
  sayln("\nESP-NOW соединение стартовало ...\n");
  NowSerial.begin(115200);
  //Serial.printf("ESP-NOW version: %d, max data length: %d\n", ESP_NOW.getVersion(), ESP_NOW.getMaxDataLen());
  sayln("Теперь можно вводить данные из последовательного порта и передавать по ESP_NOW,");
  sayln("а также выводить полученные данные в последовательный порт и на экран CYD.\n");
}

void say(char mess[])
{
  Serial.print(mess);
  //tft.print(mess);
  scroll.write(mess);
  yield(); 
}
void sayln(char mess[])
{
  Serial.println(mess);
  //tft.println(mess);
  scroll.write(String(mess));
  yield(); 
}

void say(const char mess[])
{
  Serial.print(mess);
  //tft.print(mess);
  scroll.write(String(mess));
  yield(); 
}
void sayln(const char mess[])
{
  Serial.println(mess);
  //tft.println(mess);
  scroll.write(String(mess));
  yield(); 
}

void say(int mess)
{
  Serial.print(mess);
  //tft.print(mess);
  scroll.write(String(mess));
  yield(); 
}
void sayln(int mess)
{
  Serial.println(mess);
  //tft.println(mess);
  scroll.write(String(mess));
  yield(); 
}

void say(char mess)
{
  Serial.print(mess);
  //tft.print(mess);
  scroll.write(String(mess));
  yield(); 
}
void sayln(char mess)
{
  Serial.println(mess);
  //tft.println(mess);
  scroll.write(String(mess));
  yield(); 
}

void say(String mess)
{
  Serial.print(mess);
  //tft.print(mess);
  scroll.write(mess);
  yield(); 
}
void sayln(String mess)
{
  Serial.println(mess);
  //tft.println(mess);
  scroll.write(mess);
  yield(); 
}

void loop() 
{
  while (NowSerial.available()) 
  {
    //Serial.write(NowSerial.read());
    char c = NowSerial.read();
    say(c);
  }

  while (Serial.available() && NowSerial.availableForWrite()) 
  {
    if (NowSerial.write(Serial.read()) <= 0) 
    {
      sayln("Ошибка отправки данных по ESP_NOW");
      break;
    }
  }
  // Делаем паузу на очистку мусора FreeRTOS
  delay(1);
}
