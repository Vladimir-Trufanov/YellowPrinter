/*  
 *   TestTouchscreen.ino
 *   
 *   Rui Santos & Sara Santos - Random Nerd Tutorials
    THIS EXAMPLE WAS TESTED WITH THE FOLLOWING HARDWARE:
    1) ESP32-2432S028R 2.8 inch 240×320 also known as the Cheap Yellow Display (CYD): https://makeradvisor.com/tools/cyd-cheap-yellow-display-esp32-2432s028r/
      SET UP INSTRUCTIONS: https://RandomNerdTutorials.com/cyd/
    2) REGULAR ESP32 Dev Board + 2.8 inch 240x320 TFT Display: https://makeradvisor.com/tools/2-8-inch-ili9341-tft-240x320/ and https://makeradvisor.com/tools/esp32-dev-board-wi-fi-bluetooth/
      SET UP INSTRUCTIONS: https://RandomNerdTutorials.com/esp32-tft/
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include <SPI.h>

/*  Install the "TFT_eSPI" library by Bodmer to interface with the TFT Display - https://github.com/Bodmer/TFT_eSPI
    *** IMPORTANT: User_Setup.h available on the internet will probably NOT work with the examples available at Random Nerd Tutorials ***
    *** YOU MUST USE THE User_Setup.h FILE PROVIDED IN THE LINK BELOW IN ORDER TO USE THE EXAMPLES FROM RANDOM NERD TUTORIALS ***
    FULL INSTRUCTIONS AVAILABLE ON HOW CONFIGURE THE LIBRARY: https://RandomNerdTutorials.com/cyd/ or https://RandomNerdTutorials.com/esp32-tft/   */
#include <TFT_eSPI.h>

// Install the "XPT2046_Touchscreen" library by Paul Stoffregen to use the Touchscreen - https://github.com/PaulStoffregen/XPT2046_Touchscreen
// Note: this library doesn't require further configuration
#include <XPT2046_Touchscreen.h>

TFT_eSPI tft = TFT_eSPI();

// c:\Users\tve\Documents\Arduino

// Touchscreen pins
#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define FONT_SIZE 2

// Touchscreen coordinates: (x, y) and pressure (z)
int x, y, z;

// Print Touchscreen info about X, Y and Pressure (Z) on the Serial Monitor
void printTouchToSerial(int touchX, int touchY, int touchZ) {
  Serial.print("X = ");
  Serial.print(touchX);
  Serial.print(" | Y = ");
  Serial.print(touchY);
  //Serial.print(" | Pressure = ");
  Serial.print(" | Нажми Place");
  Serial.print(touchZ);
  Serial.println();
}

// Print Touchscreen info about X, Y and Pressure (Z) on the TFT Display
void printTouchToDisplay(int touchX, int touchY, int touchZ) {
  // Clear TFT screen
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);

  int centerX = SCREEN_WIDTH / 2;
  int textY = 80;
 
  String tempText = "X = " + String(touchX);
  tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);

  textY += 20;
  tempText = "Y = " + String(touchY);
  tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);

  textY += 20;
  tempText = "Pressure = " + String(touchZ);
  tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);
}

void setup() 
{
  Serial.begin(115200);

  // Start the SPI for the touchscreen and init the touchscreen
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  // Set the Touchscreen rotation in landscape mode
  // Note: in some displays, the touchscreen might be upside down, so you might need to set the rotation to 3: touchscreen.setRotation(3);
  touchscreen.setRotation(1);

  // Start the tft display
  tft.init();
  // Set the TFT display rotation in landscape mode
  tft.setRotation(1);

  // Clear the screen before writing to it
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  
  // Set X and Y coordinates for center of display
  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;

  tft.drawCentreString("Hello, world!", centerX, 30, FONT_SIZE);
  //tft.drawCentreString("Привет, world!", centerX, 30, FONT_SIZE);
  tft.drawCentreString("Touch screen to test", centerX, centerY, FONT_SIZE);
  showpoints();
}

void showpoints() 
{
  tft.setCursor(10,  10);   tft.print("+");
  tft.setCursor(10, 230);   tft.print("+");
  tft.setCursor(310, 10);   tft.print("+");
  tft.setCursor(310,230);   tft.print("+");
}

void loop() 
{
  // Checks if Touchscreen was touched, and prints X, Y and Pressure (Z) info on the TFT display and Serial Monitor
  if (touchscreen.tirqTouched() && touchscreen.touched()) 
  {
    // Get Touchscreen points
    TS_Point p = touchscreen.getPoint();
    // Calibrate Touchscreen points with map function to the correct width and height
    x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT); 
 // y = map(p.y, 160, 3642, 1, SCREEN_HEIGHT); // 1) upP: 15=>240, 10=>y y=10*240/15=160; downP: 240=>3800, 230=>y y=230*3800/240=3642 - стало хуже, уменьшаем только нижний пказатель на 3600
 // y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT); // original
    z = p.z;

    printTouchToSerial(x, y, z);
    printTouchToDisplay(x, y, z);

    showpoints();
    delay(100);
  }
}

/*
Что за функция x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
--------------------------------------------------------
 
Суть в калибровке. Сенсорный экран при касании выдаёт «сырые» значения (например, 
от контроллера). Но эти значения не совпадают с реальными пикселями на экране — 
у сенсора своя специфика. Функция map() как раз переводит «сырые» показания в 
координаты, подходящие для отображения на экране. 

Проще говоря, функция «растягивает» или «сжимает» диапазон значений сенсора 
(200–3700) до нужного диапазона пикселей экрана (1–SCREEN_WIDTH). 

Cинтаксис:
p.x — это значение по оси X, которое сенсор отдал при касании.
200, 3700 — эмпирические границы. Их находят экспериментально: касаются крайне 
левой точки экрана и фиксируют минимальное значение (200), а при касании крайне 
правой точки — максимальное (3700).
1, SCREEN_WIDTH — диапазон, в который нужно отобразить данные. Если экран имеет
ширину SCREEN_WIDTH пикселей, то результат map() должен лежать в пределах от 1 
до SCREEN_WIDTH.
 
Пример: если сенсор при касании дал p.x = 3000, то map(3000, 200, 3700, 1, SCREEN_WIDTH)
вернёт значение, пропорциональное положению внутри экрана (например, 1.7, которое
можно округлить или использовать как индекс).

Аналогично часто встречается и вторая строка: y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT).
Для оси Y тоже определяют свои границы (240 и 3800). 

Почему такие числа? Они зависят от конкретной модели сенсора, способа подключения 
и возможной нелинейности. Иногда у сенсора есть «мёртвая зона» у края, или АЦП 
контроллера имеет смещение. 

Что делать, если калибровка «плывёт»? Можно вывести на экран маркеры крайних точек, 
записать реальные значения p.x при касании этих точек и подставить новые границы 
в map(). 

Так что эта строчка — просто удобный способ «подстроить» сырые данные сенсора 
под логику вашего графического интерфейса. 

Как проверить точность калибровки функции map()?
------------------------------------------------
Шаг 1. Визуальная проверка по ключевым точкам
Создайте тестовую программу, которая:
выводит на экран маркеры в ключевых точках (углы и центр);
при касании маркера отображает сырые (p.x, p.y) и преобразованные (x, y) координаты.

Пример кода (Arduino/ESP32):
void loop() 
{
  if (ts.touched()) 
  {
    TS_Point p = ts.getPoint();
    int x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    int y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);

    Serial.print("Raw: ");
    Serial.print(p.x);
    Serial.print(", ");
    Serial.println(p.y);
    Serial.print("Mapped: ");
    Serial.print(x);
    Serial.print(", ");
    Serial.println(y);
  }
  delay(100);
}

Шаг 2. Количественная оценка ошибки

Для каждой тестовой точки рассчитайте ошибку по формуле:

Ошибка x = x фактическая − x ожидаемая ​
Ошибка y = y фактическая​ − y ожидаемая
​
Типичные тестовые точки:

углы экрана (0,0), (SCREEN_WIDTH, 0), (0, SCREEN_HEIGHT), (SCREEN_WIDTH, SCREEN_HEIGHT);
центр экрана (SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
середины сторон.

Шаг 3. Построение карты ошибок

Разбейте экран на сетку (например, 3×3 или 5×5) и:
Коснитесь каждой точки сетки.
Запишите фактическую и ожидаемую координаты.
Рассчитайте ошибку для каждой точки.
Визуализируйте результаты в виде таблицы или графика.

Пример таблицы:

Позиция	   Ожидаемые (X,Y) Фактические (X,Y) Ошибка X	Ошибка Y
---------- --------------- ----------------- -------- --------
Левый верх (10,10)	       (8,12)	           2	      2
Центр	     (160,120)	     (158,119)	       2	      1
Правый низ (310,230)	     (305,225)	       5      	5

Шаг 4. Рассчитайте средние ошибки:

Средняя ошибка 𝑥= ∑ Ошибка 𝑥/ 𝑁

Методы улучшения точности
-------------------------

Метод 1. Уточнение границ калибровки

Если ошибка систематическая (везде смещена в одну сторону):
Коснитесь крайних точек экрана.
Запишите новые сырые значения (p.x_min, p.x_max, p.y_min, p.y_max).
Обновите параметры в map().

Метод 2. Многоточечная калибровка

Используйте более сложную функцию преобразования вместо map():

int calibratedX(int rawX) 
{
  // Линейная интерполяция между несколькими точками
  // или полиномиальная аппроксимация
}

Метод 3. Усреднение значений

Устраняет дрожание сенсора:

int getStableX() 
{
  int sum = 0;
  for (int i = 0; i < 5; i++) 
  {
    sum += ts.getPoint().x;
    delay(10);
  }
  return map(sum / 5, 200, 3700, 1, SCREEN_WIDTH);
}

Метод 4. Коррекция нелинейности

Если ошибка неравномерна по экрану, разделите экран на зоны и используйте разные коэффициенты калибровки для каждой зоны.

Практические советы:
- Повторяемость. Проводите тесты при одинаковых условиях (температура, освещение).
- Инструмент.    Используйте стилус вместо пальца для точных касаний.
- Границы.       Не калибруйте по самым краям — там часто «мёртвые зоны».
- Проверка.      После изменений повторите тесты по всей сетке.
- Сохранение.    Записывайте лучшие параметры калибровки в EEPROM или файл.

Критерии хорошей калибровки:
- максимальная ошибка < 10 пикселей;
- средняя ошибка < 5 пикселей;
- отсутствие систематических смещений;
- стабильность при повторных тестах.

Пример полного кода с фильтрацией
---------------------------------

#define CALIB_SAMPLES 5
int getFilteredValue(int raw, int minVal, int maxVal, int screenSize) 
{
  static int samples[CALIB_SAMPLES];
  static int index = 0;
  samples[index] = raw;
  index = (index + 1) % CALIB_SAMPLES;

  long sum = 0;
  for (int i = 0; i < CALIB_SAMPLES; i++) 
  {
    sum += samples[i];
  }
  int avg = sum / CALIB_SAMPLES;
  return map(avg, minVal, maxVal, 0, screenSize - 1);
}

// В основном цикле:

int x = getFilteredValue(p.x, 200, 3700, SCREEN_WIDTH);
int y = getFilteredValue(p.y, 240, 3800, SCREEN_HEIGHT);

Этот код усредняет 5 последних значений перед применением map(), что снижает влияние шумов.

*/
