/** YellowPrinter-Esp32-Arduino                            *** spriteMain.h ***
 * 
 * Представить на экране дисплея поступившие строки и уложенные в спрайт
 * 
 * v1.1.1, 16.07.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 12.06.2026
**/

#pragma once

#include <Arduino.h>
#include "inimem.h"

// sm - префикс спрайта сообщений, поступающих в CYD
#define smLINESIZE    80    // размер буфера одного сообщения
#define smLINEHEIGHT  16    // высота строки в спрайте (px)
#define smLINEOFFSET   2    // смещение текста от левого края спрайта (px)
#define smMAXLINE     13    // число строк в спрайте
#define smWIDTH      304    // ширина спрайта сообщений 
#define smHEIGHT     208    // высота спрайта
#define smTOP          0    // позиция по вертикали размещения спрайта на экране 
#define smLEFT        16    // позиция по горизонтали

TFT_eSprite stext3 = TFT_eSprite(&tft);

class TSprite_Main 
{
  public:

  TSprite_Main(); 
  void View(char* line0);
  void viewLine(char* line0);

  private:

  char lineIn[smLINESIZE];      // буфер входного сообщения
  char fill[smLINESIZE];        // заполнитель строки
  char line[smMAXLINE][smLINESIZE] =   // буфер всех сообщений экрана дисплея
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
};

TSprite_Main::TSprite_Main() 
{
  // Чистим заполнитель
  memset(fill,32,smLINESIZE-1); 
  fill[smLINESIZE-1]='\0';
};

void TSprite_Main::View(char* line0)
{
  stext3.setColorDepth(8);
  stext3.createSprite(smWIDTH, smHEIGHT);
  if (stext3.created())
  {
    getheap("Создан спрайт");
    // Заполняем буфер памяти, выделенный под спрайт, заданным цветом
    stext3.fillSprite(TFT_BLACK);
    // Отключаем перенос текста и по горизонтали и по вертикали 
    stext3.setTextWrap(false, false);
    // Определяем цвет текста с прозрачным фоном  
    stext3.setTextColor(TFT_WHITE,TFT_BLACK,true); 
    // Загружаем шрифт в память спрайта
    stext3.loadFont("HuaweiSans16");   
    // Чистим заполнитель
    memset(fill,32,smLINESIZE-1); 
    fill[smLINESIZE-1]='\0';
    getheap("Загружен фонт");

    viewLine(line0);
    stext3.pushSprite(smLEFT,smTOP);

    stext3.unloadFont();             // выгрузка шрифта из памяти
    stext3.deleteSprite();
  }
  else
  {
    Serial.println("НЕ ПОЛУЧИЛОСЬ!");
  }
}
//
void TSprite_Main::viewLine(char* line0)
{
  // В окончательном варианте работать через семафор на формирование и вывод спрайта

  uint8_t ypoint;

  // Сдвигаем строки спрайта вниз
  for (uint8_t jline = smMAXLINE-1; jline > 0; jline--) 
  {
    memcpy(line[jline],line[jline-1], sizeof(line[jline-1]));
  }
  // Заполняем 0 строку
  memset(line[0],'\0',smLINESIZE); 
  memcpy(line[0],line0,smLINESIZE);

  // Размещаем строки в спрайте
  for (uint8_t jline = 0; jline < smMAXLINE; jline++) 
  {
    ypoint=jline*smLINEHEIGHT;
    stext3.setCursor(smLINEOFFSET,ypoint);
    stext3.print(fill);
    stext3.setCursor(smLINEOFFSET,ypoint);
    stext3.print(line[jline]);
  }
}

// *********************************************************** spriteMain.h ***
