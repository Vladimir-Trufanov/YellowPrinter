// receiver_bare_minimum.ino
// Wolfgang Ewald [https://wolles-elektronikkiste.de/en/esp-now]

#include <esp_now.h>
#include <WiFi.h>

#include <ESP.h>
#include <SPIFFS.h>


#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

#include "fs_trass.h"

#include "inimem.h"
#include "spriteMain.h"
TSprite_Main ypsMain(4);

uint16_t i=0;


/*
В конце SETUP когда чистый setup
ESP.getHeapSize        = 332128
ESP.getFreeHeap        = 283892
ESP.getMinFreeHeap     = 278244
ESP.getMaxAllocHeap    = 110580
ESP.getPsramSize       = 0
ESP.getFreePsram       = 0
ESP.getMinFreePsram    = 0
ESP.getMaxAllocPsram   = 0
ESP.getFlashChipSize   = 4194304
ESP.getSketchSize      = 976752
ESP.getFreeSketchSpace = 1310720
Free space: 2185942 bytes

*/

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
  ypsMain.chsay();
  //saymem("После вывода сообщения"); 
  getmems("После вывода сообщения");
}

void setup()
{
  Serial.begin(115200);
  delay(3000); // uncomment if your serial monitor is empty

  //saymem("В начале SETUP"); 
  //getmems("В начале SETUP");

  
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  
  
  // инициализация SPIFFS
  if (!SPIFFS.begin()) 
  {
    while (1) yield();
  }  

  
  //tft.loadFont("nasalization48"); // загрузка в память шрифта
  //tft.loadFont("HuaweiSans16");     // загрузка в память шрифта
  tft.setCursor(0,0);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK,true);
  //tft.unloadFont();               // выгрузка шрифта из памяти

  Serial.println("\nПриложение запущено!");
  tft.println("Приложение запущено!");

  ypsMain.init();
  
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(messageReceived);
  
  //saymem("В конце SETUP"); 
  //getmems("В конце SETUP");
}

void getmems(char* prefix)
{
  Serial.println(prefix);

  // uint32_t getHeapSize();      //total heap size
  Serial.print("ESP.getHeapSize        = "); Serial.println(ESP.getHeapSize());
  
  // uint32_t getFreeHeap();      //available heap
  Serial.print("ESP.getFreeHeap        = "); Serial.println(ESP.getFreeHeap());
  
  // uint32_t getMinFreeHeap();   //lowest level of free heap since boot
  Serial.print("ESP.getMinFreeHeap     = "); Serial.println(ESP.getMinFreeHeap());
  
  // int32_t getMaxAllocHeap();  //largest block of heap that can be allocated at once
  Serial.print("ESP.getMaxAllocHeap    = "); Serial.println(ESP.getMaxAllocHeap());

  // uint32_t getPsramSize();
  Serial.print("ESP.getPsramSize       = "); Serial.println(ESP.getPsramSize());
  
  // uint32_t getFreePsram();
  Serial.print("ESP.getFreePsram       = "); Serial.println(ESP.getFreePsram());
  
  // uint32_t getMinFreePsram();
  Serial.print("ESP.getMinFreePsram    = "); Serial.println(ESP.getMinFreePsram());
  
  // uint32_t getMaxAllocPsram();
  Serial.print("ESP.getMaxAllocPsram   = "); Serial.println(ESP.getMaxAllocPsram());

  // inline uint32_t getCycleCount() __attribute__((always_inline));
  // Serial.print("ESP.getCycleCount      = "); Serial.println(ESP.getCycleCount());
  
  // uint32_t getFlashChipSize();
  Serial.print("ESP.getFlashChipSize   = "); Serial.println(ESP.getFlashChipSize());

  // uint32_t getSketchSize();
  Serial.print("ESP.getSketchSize      = "); Serial.println(ESP.getSketchSize());

  // uint32_t getFreeSketchSpace();
  Serial.print("ESP.getFreeSketchSpace = "); Serial.println(ESP.getFreeSketchSpace());

  // size of the compiled program
  uint32_t program_size = ESP.getSketchSize();

  // size of the file system
  uint32_t file_system_size = (uint32_t) SPIFFS.totalBytes();

  // used size of the file system
  uint32_t file_system_used = (uint32_t) SPIFFS.usedBytes();
  //uint32_t value = (uint32_t)size_t_variable;

  // free size in the flash memory
  uint32_t free_size = ESP.getFlashChipSize() - program_size - file_system_size + file_system_used;
  Serial.print("Free space: "); Serial.print(free_size); Serial.println(" bytes");

}

void loop(){}
 
