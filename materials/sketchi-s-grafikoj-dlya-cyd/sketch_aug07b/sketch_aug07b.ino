/*  Rui Santos & Sara Santos - Random Nerd Tutorials
    THIS EXAMPLE WAS TESTED WITH THE FOLLOWING HARDWARE:
    1) ESP32-2432S028R 2.8 inch 240×320 also known as the Cheap Yellow Display (CYD): https://makeradvisor.com/tools/cyd-cheap-yellow-display-esp32-2432s028r/
      SET UP INSTRUCTIONS: https://RandomNerdTutorials.com/cyd/
    2) REGULAR ESP32 Dev Board + 2.8 inch 240x320 TFT Display: https://makeradvisor.com/tools/2-8-inch-ili9341-tft-240x320/ and https://makeradvisor.com/tools/esp32-dev-board-wi-fi-bluetooth/
      SET UP INSTRUCTIONS: https://RandomNerdTutorials.com/esp32-tft/
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

TFT_eSPI tft = TFT_eSPI();

// Пины сенсорного экрана
#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define FONT_SIZE 4

// Координаты сенсорного экрана: (x, y) и давление (z)
int x, y, z;


// Пункт меню
const char* menuItems[] = 
{
    "Start123",
    "Settings",
    "Info1234",
    "Exit1234"
};
int menuSize = 4;  // sizeof(menuItems) / sizeof(menuItems);
int selected = 0;  // Индекс текущего выбранного пункта 



// Вывод информации о сенсорном экране о X, Y и давлении (Z) в мониторе последовательного порта
void printTouchToSerial(int touchX, int touchY, int touchZ) {
  Serial.print("X = ");
  Serial.print(touchX);
  Serial.print(" | Y = ");
  Serial.print(touchY);
  Serial.print(" | Pressure = ");
  Serial.print(touchZ);
  Serial.println();
}

// Вывод информации о сенсорном экране о X, Y и давлении (Z) на TFT дисплее
void printTouchToDisplay(int touchX, int touchY, int touchZ) {
  // Очистка экрана TFT
  //tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);

  int centerX = SCREEN_WIDTH / 2;
  int textY = 80;
 
  String tempText = "X = " + String(touchX);
  tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);

  textY += 20;
  tempText = "Y = " + String(touchY);
  tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);

  textY += 20;
  tempText = "Pressure  = " + String(touchZ);
  tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);
}

void setup() 
{
  Serial.begin(115200);
  delay(5000);
  Serial.print("menuSize = "); Serial.println(menuSize);

  // Начать использовать SPI для сенсорного экрана и инициализировать сенсорный экран
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  // Установить поворот сенсорного экрана в ландшафтный режим
  // Примечание: на некоторых дисплеях сенсорный экран может быть перевернут, поэтому может потребоваться установить поворот в 3: touchscreen.setRotation(3);
  touchscreen.setRotation(1);

  // Начать использовать TFT дисплей
  tft.init();
  // Установить поворот TFT дисплея в ландшафтный режим
  tft.setRotation(1);
  //tft.loadFont("HuaweiSans16");   
  tft.loadFont("nasalization48");   


  // Очистить экран перед записью на него
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  
  // Установить координаты X и Y для центра дисплея
  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;

  //tft.drawCentreString("Hello, world!", centerX, 30, FONT_SIZE);
  tft.drawCentreString("Touch screen to test", centerX, centerY, FONT_SIZE);

    // Отрисовка меню
    int y=0;
    
    tft.setCursor(0, 0);
    for (int i = 0; i < menuSize; i++) 
    {
        if (i == selected) 
        {
            y=i*16;
            tft.setCursor(0, y);
            tft.print(">");  // Маркер выбора
            tft.print(menuItems[i]);
        } 
        else 
        {
            y=i*16;
            tft.setCursor(0, y);
            tft.print(" ");
            //tft.setCursor(0, 0);
            tft.print(menuItems[i]);
        }
    }

  
}

void loop() 
{
  // Проверка, коснулся ли кто-то сенсорного экрана, и вывод информации о X, Y и давлении (Z) на TFT дисплей и в монитор последовательного порта
  if (touchscreen.tirqTouched() && touchscreen.touched()) 
  {
    // Получение координат сенсорного экрана
    TS_Point p = touchscreen.getPoint();
    // Калибровка координат сенсорного экрана с помощью функции map к правильной ширине и высоте
    x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    z = p.z;

    printTouchToSerial(x, y, z);
    printTouchToDisplay(x, y, z);

        /*
        int x = tsc.getX();
        int y = tsc.getY();
        tft.setCursor(0, 0);
        tft.print("X: ");
        tft.print(x);
        tft.print(" Y: ");
        tft.print(y);

        // Обработка выбора (пример)
        if (y < 3) 
        {
            tft.println("Вы выбрали: " + String(y));
            delay(1000);
            tft.clear();
            tft.println("Меню готово");
        }
        */

    delay(100);
  }
}
