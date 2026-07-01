/*
**  Author: Aniket Biswas
**  Github: https://github.com/xunicatt
**  
**  Example for TFT_eSPI_Scroll library
**
**  Contribute to improve the library
*/

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <TFT_eSPI_Scroll.h>
#include <4bit.h>


TFT_eSPI tft;
TFT_eSPI_Scroll scroll;
int count = 1;


#include "ESP32_NOW_Serial.h"
#include "MacAddress.h"
#include "WiFi.h"
#include "esp_wifi.h"

// 0: AP mode, 1: Station mode
#define ESPNOW_WIFI_MODE_STATION 1
// Channel to be used by the ESP-NOW protocol
#define ESPNOW_WIFI_CHANNEL 1

#define ESPNOW_WIFI_MODE WIFI_STA     // WiFi Mode
#define ESPNOW_WIFI_IF   WIFI_IF_STA  // WiFi Interface


void setup()
{
    Serial.begin(115200);
    delay(2000);



  // инициализация SPIFFS
  if (!SPIFFS.begin()) 
  {
    while (1) yield();
  }  



    
    // Initializing tft_espi
    tft.init(); 
    tft.setRotation(1);   

    // Initializing the tft_espi_scroll int 1bit B/W
    if(scroll.init(&tft, 2) != NO_ERROR)
    {
        Serial.println("Failed... Reseting...");        
        return;
    }

    
  // По умолчанию определяем режим станции
  say("1 Режим WiFi: ");
  scroll.write("2 Режим WiFi: ");
  yield(); 

  sayln(ESPNOW_WIFI_MODE == WIFI_AP ? "собственный WiFi" : "как станция");
  WiFi.mode(ESPNOW_WIFI_MODE);

  say("Канал: ");
  sayln(ESPNOW_WIFI_CHANNEL);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);

    delay(3000);

    String data;

    for(int i = 1; i <= 10; i++)
    {
        data = String("Count: ") + String(i); 
        scroll.write(data);
     
        //must be called for longer loops
        //otherwise watchdog will reset the mcu
        yield(); 
       delay(100);
    }
   

    // reseting
    scroll.reset();


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


    // for 4bit
    //scroll.init(&tft, 4, 4);
    //scroll.setColor(COLOR_BLUE_4B, COLOR_RED_4B);
}

void loop()
{
    //scroll.write("2 x " + String(count) + " = " + String(2*count++));
    //delay(100);
}


void say(char mess[])
{
  scroll.write("char mess[]");
  yield(); 

  Serial.print(mess);
  //tft.print(mess);
  scroll.write(String(mess));
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
