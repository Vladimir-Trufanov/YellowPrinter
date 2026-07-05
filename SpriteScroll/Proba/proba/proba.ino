#include <TFT_eSPI.h>
#include <SPI.h>       // this is needed for display
#include <Wire.h>      // this is needed for FT6206
#include <cstring>

// The display also uses hardware SPI, plus #9 & #10
/*
  Выводы дисплея
  --------------
  MISO       (TFT_MISO)	     GPIO 12
  MOSI       (TFT_MOSI)	     GPIO 13
  SCKL       (TFT_SCLK)	     GPIO 14
  CS         (TFT_CS)        GPIO 15
  DC         (TFT_DC)	       GPIO 2

*/
#define TFT_CS 15
#define TFT_DC 2
#define TFT_MOSI 13
#define TFT_SCLK 14

//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library with default width and height

TFT_eSprite sprite = TFT_eSprite(&tft);  // Создание экземпляра класса Sprite

String text = "Hello world234! This is Sprite";

int32_t cursor_x = 0;
uint8_t textpos = 0;

void setup(void) {
  Serial.begin(115200);       // Инициализация последовательной связи для вывода отладочных сообщений
  tft.begin();                // Инициализация экрана
  tft.setRotation(1);         // Установка ориентации экрана
  tft.fillScreen(TFT_BLACK);  // Заполнение экрана черным цветом

  sprite.setTextSize(2);           // Установка размера шрифта для спрайта
  sprite.setTextColor(TFT_GREEN);  // Установка цвета текста в зелёный

  sprite.createSprite(200 + 15, 16);               // Создание спрайта с заданной шириной и высотой
  sprite.setCursor(0, 0);                          // Установка начальной позиции курсора в спрайте
  sprite.fillRect(0, 0, 200 + 15, 16, TFT_BLACK);  // Заполнение фона спрайта черным цветом
}

void loop() {
  cursor_x--;  // Декрементация координаты X курсора
  if (cursor_x == 70) {
    sprite.setCursor(190, 0);  // Установка курсора в конец строки в спрайте
    cursor_x = 190;            // Установка координаты X курсора в конечное значение
    textpos = 0;               // Сброс позиции текста для новой строки
  }
  sprite.scroll(0, -1);  // Прокрутка содержимого спрайта влево
  if (cursor_x <= 200 && text[textpos] != '\0') {
    sprite.setCursor(cursor_x, 0);   // Установка курсора на новую позицию в спрайте
    sprite.print(text[textpos++]);   // Вывод текущего символа в спрайт и инкрементация позиции текста
    cursor_x = sprite.getCursorX();  // Получение текущей координаты X курсора
  }
  sprite.pushSprite(0, 10);  // Вывод спрайта на экран с учетом смещения по Y
  delay(1000);
}

void scrollSprite() {
  while (true) {
    sprite.print(text[textpos]);     // Вывод текущего символа в спрайт
    textpos++;                       // Инкрементация позиции текста
    cursor_x = sprite.getCursorX();  // Получение текущей координаты X курсора
    sprite.pushSprite(0, 10);        // Вывод спрайта на экран с учетом смещения по Y
    if (cursor_x > 200) {            // Проверка достижения конца строки в спрайте
      break;                         // Прерывание цикла прокрутки
    }
  }
}