/** Arduino-Esp32-CAM                              *** ControllerMemory.ino ***
 * 
 *                          Показать информацию по встроенной и внешней памяти, 
 *                                       и сведения о системе контроллера ESP32
 * 
 * v1.0, 20.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 20.10.2024
 * 
**/

/*
 Esp.h - ESP31B-specific APIs
 Copyright (c) 2015 Ivan Grokhotkov. All rights reserved.
*/

char buffer[60]; // буфер сообщения

void setup() 
{
   Serial.begin(115200);
   delay(5000);
   Serial.println("");

   // Получаем полный размер кучи в памяти
   printf("Общий размер ВСТРОЕННОЙ памяти:     %u\n", ESP.getHeapSize());
   // Количество доступной кучи в памяти
   printf("Оставшаяся доступная память в куче: %u\n", ESP.getFreeHeap());
   // Самый низкий уровень свободной кучи с момента загрузки
   printf("Минимальная свободная с загрузки:   %u\n", ESP.getMinFreeHeap());
   // Размер самого большого блока кучи, который может быть выделен. 
   printf("Самый большой блок для выделения:   %u\n", ESP.getMaxAllocHeap());
   // Размер текущего скетча в виде беззнакового 32-битного целого числа
   printf("Размер текущего скетча:             %u\n", ESP.getSketchSize());

   Serial.println("");
   // Размер флэш-чипа в байтах. Этот параметр может быть меньше фактического размера.
   printf("Флэш-чип: SPI Flash File Storage    %u\n", ESP.getFlashChipSize());
   // Размер общей кучи SPI PSRAM
   printf("Общий размер SPI PSRAM:             %u\n", ESP.getPsramSize());
   // Количество свободной PSRAM
   printf("Количество свободной PSRAM:         %d\n", ESP.getFreePsram());
   // Минимальный размер свободной памяти в SPI RAM
   printf("Минимум свободной SPI PSRAM:        %d\n", ESP.getMinFreePsram());
   // Размер самого большого блока PSRAM, который может быть выделен
   printf("Самый большой блок для выделения:   %d\n", ESP.getMaxAllocPsram());
   // Размер свободного пространства для скетча в виде 32-битного целого
   printf("Cвободное пространство для скетча:  %u\n", ESP.getFreeSketchSpace());

   Serial.println("");
   // Модель ЧИПа
   sprintf(buffer,
          "Модель контроллера:               (%s)\n", ESP.getChipModel());
   Serial.print(buffer);
   // Номер ревизии чипа
   printf("Номер версии чипа:                  %d\n", ESP.getChipRevision());
   // Количество процессорных ядер в микроконтроллере
   printf("Количество процессорных ядер:       %d\n", ESP.getChipCores());
   // Частота процессора в МГц в виде беззнакового 8-битного целого числа
   printf("Частота процессора в МГц:           %d\n", ESP.getCpuFreqMHz());
   // getFlashChipSpeed
   printf("getFlashChipSpeed:                  %u\n", ESP.getFlashChipSpeed());

   
   Serial.println("");
   // version of ESP-IDF
   sprintf(buffer,
          "Версия ESP-IDF:                   (%s)\n", ESP.getSdkVersion());
   Serial.print(buffer);

   // version of this core
   sprintf(buffer,
          "Версия ESP32 в IDE Arduino:       (%s)\n", ESP.getCoreVersion());
   Serial.print(buffer);
   Serial.println("");
}

void loop() {}

// *************************************************** ControllerMemory.ino ***
