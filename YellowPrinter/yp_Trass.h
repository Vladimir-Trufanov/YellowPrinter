/** Esp32-2432S028R CYD                                      *** yp_Trass.h ***
 * 
 *                          Обеспечить вывод сообщений в последовательный порт, 
 *                                      информационную строку CYD и на SD-карту
 *                                                     
 * v2.2.2, 26.08.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 24.01.2026
**/

#pragma once 

#include "FS.h"
//#include <SD_MMC.h>
#include "inimem.h"
#include "yp_SPIFFS.h"


void tft_say() 
{
  char fill[smLINESIZE];       
  memset(fill,32,smLINESIZE-1); 
  fill[smLINESIZE-1]='\0';
  tft.setCursor(17, 223);
  tft.print(fill);
}

#define say(format, ...) \
{ \
  char buffer[smLINESIZE]; \
  snprintf(buffer, sizeof(buffer), format, ##__VA_ARGS__); \
  Serial.print(buffer); \
  tft_say(); \
  tft.setCursor(17,223); \
  tft.print(buffer); \
}

#define sayln(format, ...) \
{ \
  char buffer[256]; \
  snprintf(buffer, sizeof(buffer), format, ##__VA_ARGS__); \
  Serial.println(buffer); \
  tft_say(); \
  tft.setCursor(223,16); \
  tft.print(buffer); \
}
  //if (logfile) { \
  //  logfile.print(buffer); \
  //} \

  //if (logfile) { \
  //  logfile.println(buffer); \
  //} \

// ****************************************************************************
// *  Инициировать вывод локальных сообщений в информационную строку дисплея  *
// ****************************************************************************
void tft_init() 
{
  tft.init();
  tft.setRotation(1);      
  tft.fillScreen(TFT_NAVY); 
  tft.setTextWrap(false, false);                // отключили перенос текста и по горизонтали и по вертикали 
  tft.setTextColor(TFT_YELLOW,TFT_NAVY,true);  // определили цвет текста с прозрачным фоном  
  // Загружаем шрифт в память
  #ifdef FontFromFile
    tft.loadFont(HuaweiSans16);   
  #else
    tft.loadFont("HuaweiSans16");   
  #endif
  #ifdef FontFromSPIFFS
  // Инициализируем SPIFFS
  iniSPIFFS();
  #endif
}
// ****************************************************************************
// *           Показать установленные настройки камеры, видео и другие        *
// ****************************************************************************
void sayconfig() 
{
  /*
  say("============ Установленные настройки камеры, видео и другие =========\n");
  //say("Название камеры                      %s\n",      devname);
  say("Название контроллера                 %s\n",      devname);
  say("Размер кадра                         %d\n",      framesize);
  say("Качество                             %d\n",      quality);
  say("Количество буферов для кадров        %d\n",      buffersconfig);
  say("Размер видео в секундах              %d\n",      avi_length);
  say("Интервал между записями кадров (ms)  %d\n",      frame_interval);
  say("Ускорение воспроизведения            %d\n",      speed_up_factor);
  say("Интервал между кадрами в потоке (ms) %d\n",      stream_delay);
  say("TIMEZONE                             %d, %s\n",  TIMEZONE.length(), TIMEZONE.c_str());
  say("Сеть WiFi                            %s\n",      ssid);
  say("Тип карты SD_MMC                     "); 
  if (ncardType == 1)                       sayln("MMC")      
  else if (ncardType == 2)                  sayln("SDSC") 
  else if (ncardType == 3)                  sayln("SDHC")
  else if (ncardType == 4)                  sayln("неопределён")
  else                                      sayln("карта не подключена");
  say("Ёмкость SD_MMC-карты                 %llu MB\n", cardSize);
  */
}

// ************************************************************* yp_Trass.h ***
