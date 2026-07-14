/** YellowPrinter-Esp32-Arduino                            *** spriteMain.h ***
 * 
 * Представить на экране дисплея поступившие строки и уложенные в спрайт
 * 
 * v1.0.0, 08.07.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 12.04.2024
**/

#pragma once

#include <Arduino.h>
#include "inimem.h"

TFT_eSprite stext3 = TFT_eSprite(&tft); // Sprite object stext3
char* IntToChar(uint16_t numbIn); 
void viewLine(uint16_t i);

//bool yessMain=false;   // true - создан спрайт вывода сообщений

char fill[80];  // заполнитель строки
char line[][80] = 
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

void sprite_Main(uint16_t i)
{
  // Create a sprite
  stext3.setColorDepth(8);
  stext3.createSprite(304, 208);
  // Delete the sprite to free up the RAM
  if (stext3.created())
  {
    getheap("Создан спрайт");
    //yessMain=true; 
    // Заполняем буфер памяти, выделенный под спрайт, заданным цветом
    stext3.fillSprite(TFT_BLACK);
    // Отключаем перенос текста и по горизонтали и по вертикали 
    stext3.setTextWrap(false, false);
    // Определяем цвет текста с прозрачным фоном  
    stext3.setTextColor(TFT_WHITE,TFT_BLACK,true); 
    // Загружаем шрифт в память спрайта
    stext3.loadFont("HuaweiSans16");   
    // Чистим заполнитель
    memset(fill,32,79); 
    fill[79]='\0';
    getheap("Загружен фонт");

    viewLine(i);
    stext3.pushSprite(16,0);

    stext3.unloadFont();             // выгрузка шрифта из памяти
    stext3.deleteSprite();
  }
  else
  {
    Serial.println("НЕ ПОЛУЧИЛОСЬ!");
  }
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
}


/*
class TSprite_Main 
{
  public:

  TSprite_Main(int iQueueSize=4); 
  void init(); 
  void sayLine(int16_t x, int16_t y, char* line); 
  void chsay(); 
  void clrScr(bool color); 

  private:

  TFT_eSPI tft = TFT_eSPI();
  TFT_eSprite stext3 = TFT_eSprite(&tft); 

  uint16_t i=0;
  char line[128];  // строка спрайта
  char fill[128];  // заполнитель строки


};


  TSprite_Main::TSprite_Main(int iQueueSize) 
  {
    // Чистим заполнитель
    memset(fill,32,127); 
    fill[127]='\0';
    //
    int t=iQueueSize;
  };

  void TSprite_Main::clrScr(bool color) 
  {
    bool c=color;
  }

  void TSprite_Main::init() 
  {
    // Задаем среднюю глубину цвета и создаем спрайт
    // заданного размера в пикселах в оперативной памяти
    stext3.setColorDepth(1);
    //stext3.createSprite(200, 110);
    stext3.createSprite(288, 224);
    //stext3.createSprite(244, 208);
    //stext3.createSprite(280, 160);
    // Заполняем буфер памяти, выделенный под спрайт, заданным цветом
    stext3.fillSprite(TFT_BLACK);
    // Отключаем перенос текста и по горизонтали и по вертикали 
    stext3.setTextWrap(false, false);
    // Определяем цвет текста с прозрачным фоном  
    stext3.setTextColor(TFT_WHITE,TFT_BLACK,true); 
    // Загружаем шрифт в память спрайта
    stext3.loadFont("HuaweiSans16");     // загрузка в память шрифта
    //stext3.loadFont("nasalization48");     
  }
  
  void TSprite_Main::sayLine(int16_t x, int16_t y, char* line) 
  {
    // Устанавливаем позицию курсора для вывода текста в заданной позиции спрайта
    stext3.setCursor(x,y);
    stext3.print(fill);
    stext3.setCursor(x,y);
    stext3.print(line); 
  }

  // Так как предполагается вывод всего 2 строк, то 
  // вначале выводим прежнее значение в нижней части
  void TSprite_Main::chsay() 
  {
    char chi[6]="Чи";
    char exclam[2]="!";

    memset(line,'\0',128); 
    strcat(line,chi); 
    strcat(line,IntToChar(i)); 
    strcat(line,exclam); 
    //sayLine(10,55,line);
    sayLine(10,25,line);

    // Выше складываем новое значение
    i++;
    memset(line,'\0',128); 
    strcat(line,chi); 
    strcat(line,IntToChar(i)); 
    strcat(line,exclam); 
    sayLine(10,10,line);
    
    // Выталкиваем спрайт на дисплей с заданной позиции
    //stext3.pushSprite(10,60);
    stext3.pushSprite(16,16);
  }
  */

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


  // *********************************************************** spriteMain.h ***
