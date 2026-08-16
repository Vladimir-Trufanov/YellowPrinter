/** Arduino, ESP32, C/C++ ************************************** yp_Trass.h ***
 * 
 *                           -- Обеспечить вывод сообщений в последовательный порт 
 *                                               и запись лог-файла на SD-карту
 *                                                     
 * v2.2.1, 14.06.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 24.01.2026
**/

#pragma once 

#include "esp_psram.h"  // для esp_psram_get_size();

// Подсчитать число символов UTF-8 в последовательности char* 
size_t utf8len(const char* str); 
// Показать состояние памяти с заданным префиксом  
void saymem(char* text); 

#define say(format, ...) \
  { \
    char buffer[256]; \
    snprintf(buffer, sizeof(buffer), format, ##__VA_ARGS__); \
    Serial.print(buffer); \
  }

#define sayln(format, ...) \
  { \
    char buffer[256]; \
    snprintf(buffer, sizeof(buffer), format, ##__VA_ARGS__); \
    Serial.println(buffer); \
  }

// ****************************************************************************
// *              Показать состояние памяти с заданным префиксом              *
// ****************************************************************************
// Специальные сообщения - это сообщения по использованию памяти, по времени ... 
#define isSAYLOG true   // true - вести файл дублирования сообщений
bool isSAY=true;        // true - разрешить вывод неспециальных сообщений
bool isSAYMEM=true;     // true - разрешить трассировку состояния памяти

void saymem(char* text) 
{
  if (isSAYMEM)
  {
    // Запоминаем состояние разрешения на вывод сообщений
    bool oldSay=isSAY;       
    // Разрешаем вывод сообщений
    isSAY=true;      
    // Определяем наибольшее число символов текста вместе с пробелами
    // и дополняем текст пробелами слева 
    int nfill=48;   
    int j=nfill-utf8len(text)-2;
    for (int i = 0; i<j; i++) 
    {
      say(" "); 
    }
    // Выводим специальное сообщение по памяти
    /**
     * xPortGetCoreID()        - функция возвращает номер ядра, на котором выполняется текущая задача
     * uxTaskPriorityGet(NULL) - возвращает приоритет текущей задачи (задачи, из которой была вызвана функция)
     * ESP.getFreeHeap()       - возвращает размер свободной кучи (heap) в байтах
     * ESP.getHeapSize()       - возвращает полный размер внутренней оперативной памяти в байтах (ОЗУ)
     * ESP.getFreePsram()      - свободный объём внешней оперативной памяти PSRAM
     * ESP.getPsramSize()      - полный объём внешней оперативной памяти PSRAM
    **/
    size_t psram_size = esp_psram_get_size();
    printf("Размер PSRAM: %zu bytes", psram_size);
    say("[%s]",text);
    say(" ядро: %d,",          int(xPortGetCoreID()));        say(" приоритет: %d, ", int(uxTaskPriorityGet(NULL)));
    say("свободная куча %6d ", uint32_t(ESP.getFreeHeap()));  say("от ОЗУ %6d, ",     uint32_t(ESP.getHeapSize()));
    say("FreePSRAM %6d ",      uint32_t(ESP.getFreePsram())); say("от FLASH %6d",     uint32_t(ESP.getPsramSize()));
    sayln(" ");
    // Восстанавливаем состояние разрешения на вывод сообщений
    isSAY=oldSay;      
  }
}
// ****************************************************************************
// *        Подсчитать число символов UTF-8 в последовательности char*        *
// * https://stackoverflow.com/questions/4063146/getting-the-actual-length-of-a-utf-8-encoded-stdstring
// ****************************************************************************
size_t utf8len(const char* str) 
{
  size_t len = 0;
  for (size_t i = 0; *str != 0; ++len) 
  {
    int v01 = ((*str & 0x80) >> 7) & ((*str & 0x40) >> 6);
    int v2 = (*str & 0x20) >> 5;
    int v3 = (*str & 0x10) >> 4;
    str += 1 + ((v01 << v2) | (v01 & v3));
  }
  return len;
}

// ************************************************************* yp_Trass.h ***
