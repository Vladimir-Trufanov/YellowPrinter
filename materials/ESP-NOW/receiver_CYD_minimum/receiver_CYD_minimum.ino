// receiver_bare_minimum.ino
// Wolfgang Ewald [https://wolles-elektronikkiste.de/en/esp-now]

#include <esp_now.h>
#include <WiFi.h>

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

#include "inimem.h"
#include "spriteMain.h"
TSprite_Main ypsMain(4);

uint16_t i=0;


void messageReceived(const esp_now_recv_info *info, const uint8_t* incomingData, int len)
{
  //tft.println("\nПрием: ");
  Serial.println("\nПрием: ");
  for(int i=0; i<len; i++)
  {
    Serial.print((char)incomingData[i]);
    tft.print(((char)incomingData[i]));
  }
  Serial.println();
  ypsMain.say();
}

void setup()
{
  Serial.begin(115200);
  delay(3000); // uncomment if your serial monitor is empty
   
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  // инициализация SPIFFS
  if (!SPIFFS.begin()) 
  {
    while (1) yield();
  }  

  
  //tft.loadFont("nasalization48"); // загрузка в память шрифта
  tft.loadFont("HuaweiSans16");     // загрузка в память шрифта
  tft.setCursor(0,0);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK,true);
  //tft.unloadFont();               // выгрузка шрифта из памяти

  Serial.println("\nПриложение запущено!");
  tft.println("\nПриложение запущено!");

  ypsMain.init();
  
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(messageReceived);
}

void loop(){}
 
