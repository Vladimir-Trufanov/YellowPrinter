// xsprite_v3.ino

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

char fill[80];  // заполнитель строки

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
  tft.fillScreen(TFT_NAVY);
  
  // инициализация SPIFFS
  if (!SPIFFS.begin()) 
  {
    while (1) yield();
  }  

  /*
  // Create a sprite
  stext3.setColorDepth(8);
  stext3.createSprite(32,32);
  // Заполняем буфер памяти, выделенный под спрайт, заданным цветом
  stext3.fillSprite(TFT_BLACK);
  // Отключаем перенос текста и по горизонтали и по вертикали 
  stext3.setTextWrap(false, false);
  // Определяем цвет текста с прозрачным фоном  
  stext3.setTextColor(TFT_WHITE,TFT_BLACK,true); 
  */
  
  // Create a sprite
  stext3.setColorDepth(8);
  stext3.createSprite(304, 208);
  // Заполняем буфер памяти, выделенный под спрайт, заданным цветом
  stext3.fillSprite(TFT_BLACK);
  // Отключаем перенос текста и по горизонтали и по вертикали 
  stext3.setTextWrap(false, false);
  // Определяем цвет текста с прозрачным фоном  
  stext3.setTextColor(TFT_WHITE,TFT_BLACK,true); 
  // Загружаем шрифт в память спрайта
  stext3.loadFont("HuaweiSans16");   
  //stext3.unloadFont();             // выгрузка шрифта из памяти
  getheap("Создан спрайт");

  // Чистим заполнитель
  memset(fill,32,79); 
  fill[79]='\0';

  getheap("Загружен фонт");
}

uint16_t i=0;

char line[][128] = 
{
    "Text",
    "Привет Hello world",
    "Line 2",
    "0123456 10 123456 20 123456 30 123456 40",
    "04 Это пробный текст на русском языке для CYD",   
    "0123456 10 123456 20 123456 30 123456 40 123456 50 123456 60",
    "06 Это пробный текст на русском языке для CYD",   
    "0123456 10 123456 20 123456 30 123456 40 123456 50 123456 60",
    "08 Это пробный текст на русском языке для CYD",   
    "0123456 10 123456 20 123456 30 123456 40 123456 50 123456 60",
    "10 Это пробный текст на русском языке для CYD",   
    "11 0123456 10 123456 20 123456 30 123456 40 123456 50 123456 60",
    "12 0123456 10 123456 20 123456 30 123456 40 123456 50 123456 60",
};

void loop()
{
  //stext3.pushSprite(288,0);
  
  viewLine(i);
  stext3.pushSprite(16,0);
  getheap("Цикл пройден ");
  i++;
  
  delay(5000);
}

// ****************************************************************************
// *            Преобразовать беззнаковое  целое в строку символов            *
// ****************************************************************************
char charNumby[6]; // char[5]+'\0'
char* IntToChar(uint16_t numbIn) 
{
  uint16_t numby=numbIn;
  memset(charNumby,'\0',6); 
  if (numby>65534) numby=0;
  String(numby).toCharArray(charNumby,6);
  return charNumby;
}


void viewLine(uint16_t i)
{

  // В окончательном варианте работать через семафор на формирование и вывод спрайта
  
  uint8_t ydelta=16;
  uint8_t ypoint;
  char chi[] = "Число = ";

  // Сдвигаем строки спрайта вниз
  for (uint8_t jline = 12; jline > 0; jline--) 
  {
    memcpy(line[jline],line[jline-1], sizeof(line[jline-1]));
  }
  // Заполняем 0 строку
  memset(line[0],'\0',76); 
  memcpy(line[0],chi, sizeof(chi));
  memcpy(line[0],IntToChar(i), sizeof(IntToChar(i)));
 
  // Размещаем строки в спрайте
  for (uint8_t jline = 0; jline < 13; jline++) 
  {
    ypoint=jline*ydelta;
    stext3.setCursor(2,ypoint);
    stext3.print(fill);
    stext3.setCursor(2,ypoint);
    stext3.print(line[jline]);
  }

  /*
  stext3.setCursor(2,0);
  stext3.print(fill);
  stext3.setCursor(2,0);
  stext3.print("Число = "); stext3.print(i); stext3.print("!");
  */
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
