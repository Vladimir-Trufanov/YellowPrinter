// xsprite.ino

#include <esp_now.h>
#include <WiFi.h>
#include <ESP.h>
#include <SPIFFS.h>
//#include <MemoryFree.h>
#include "inimem.h"

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite stext3 = TFT_eSprite(&tft); // Sprite object stext3

void getheap(char* prefix)
{
  Serial.print(prefix); Serial.print(": ");
  // uint32_t getHeapSize();      //total heap size
  //Serial.print("HeapSize = "); Serial.print(ESP.getHeapSize());
  //Serial.print("; ");
  // uint32_t getFreeHeap();      //available heap
  Serial.print("FreeHeap = "); Serial.println(ESP.getFreeHeap());
  //Serial.print("Свободной памяти: "); Serial.println(getFreeMemory());
}

void setup()
{
  Serial.begin(115200);
  delay(3000); // uncomment if your serial monitor is empty
  getheap("setup        ");
  //getheap("Создан спрайт");
  //getheap("Загружен фонт");
  //getheap("Цикл пройден ");

  tft.init();
  tft.setRotation(1);      
  tft.fillScreen(TFT_YELLOW);
  
  // инициализация SPIFFS
  if (!SPIFFS.begin()) 
  {
    while (1) yield();
  }  

  // Create a sprite
  stext3.setColorDepth(8);
  stext3.createSprite(304, 208);
  stext3.fillSprite(TFT_BLACK);
  stext3.setTextColor(TFT_WHITE);    // White text, no background
  stext3.setTextWrap(false, false);
  getheap("Создан спрайт");

  stext3.loadFont("HuaweiSans16");   // загрузка в память шрифта
  //stext3.unloadFont();             // выгрузка шрифта из памяти
  getheap("Загружен фонт");
}

uint16_t i=0;
void loop()
{
  stext3.setCursor(2,0);
  stext3.print("Число = "); stext3.print(i); stext3.print("!");
  i++;
  stext3.setCursor(2,16);
  stext3.println("0123456 10 123456 20 123456 30 123456 40 123456 50 123456 60 123456 70 123456 80 123456 20 123456 90 12345 100");
  stext3.setCursor(2,32);
  stext3.println("Это пробный текст на русском языке для КОНТРОЛЛЕРА CYD    60 123456 70 123456 80 123456 20 123456 90 12345 100");
  stext3.pushSprite(16,0);
  getheap("Цикл пройден ");
  delay(5000);
}

/*
Объявление спрайта в PROGMEM
Допустим, у вас монохромный спрайт 16×16 в виде массива байтов (битовая карта):

cpp
const uint8_t mySprite[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x18, 0x3C, 0x7E, 0xFF, 0xFF, 0x7E, 0x3C, 0x18,
  // ... остальные 240 байт
};
Если спрайтов несколько, удобно сделать массив указателей:

cpp
const uint8_t spriteA[] PROGMEM = { / * данные * / };
const uint8_t spriteB[] PROGMEM = { / * данные * / };

const uint8_t* const sprites[] PROGMEM = {
  spriteA,
  spriteB
};
Обратите внимание: сам массив указателей тоже помещаем в PROGMEM, иначе он будет лежать в RAM.

Чтение данных из PROGMEM (самый важный момент)
Нельзя просто делать mySprite[i] — это будет неправильно адресовать Flash. Используйте функции pgm_read_* из <avr/pgmspace.h>:

pgm_read_byte(&mySprite[i]) — для uint8_t
pgm_read_word(&mySprite[i]) — для uint16_t и т.п.
Пример чтения одного байта:

cpp
uint8_t pixel = pgm_read_byte(&mySprite[index]);
Пример перебора всего спрайта:

cpp
for (size_t i = 0; i < sizeof(mySprite); ++i) {
  uint8_t byteData = pgm_read_byte(&mySprite[i]);
  // обработка byteData
}
Если у вас массив указателей на спрайты
Чтобы получить указатель на нужный спрайт из массива sprites, тоже нужен pgm_read:

cpp
// Читаем указатель из PROGMEM в RAM
const uint8_t* pSprite = (const uint8_t*)pgm_read_word(&sprites[0]);

// Теперь читаем данные спрайта
uint8_t b = pgm_read_byte(&pSprite[i]);
Пример для TFT_eSPI и спрайтов
TFT_eSPI умеет рисовать из RAM, но не имеет «прямого» вызова «нарисуй спрайт из PROGMEM». Поэтому типичный подход:

Объявить спрайт в PROGMEM.
При необходимости (например, при инициализации) скопировать его во временный буфер в RAM (если он помещается).
Передать этот буфер в функции библиотеки.
cpp
#include <avr/pgmspace.h>
#include <TFT_eSPI.h>

TFT_eSPI tft;

const uint8_t smallSprite[] PROGMEM = {
  0xFF, 0x81, 0xA5, 0x81,
  0xFF, 0x81, 0xA5, 0x81
};

void setup() {
  tft.init();
  tft.fillScreen(TFT_BLACK);

  // Временный буфер в RAM — только для маленького спрайта!
  const size_t size = sizeof(smallSprite);
  uint8_t buffer[size];

  // Копируем из Flash в RAM
  memcpy_P(buffer, smallSprite, size);

  // Рисуем через TFT_eSPI (пример: drawRGBBitmap или свой код)
  // tft.drawRGBBitmap(x, y, buffer, w, h);
}
*/
