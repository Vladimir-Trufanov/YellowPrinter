## [Скетчи с графикой для CYD](#)

### [Репозитарий библиотек для CYD: LovyanGFX, TFT_eSPI, LVGL, SD](https://github.com/embedded-kiddie/Arduino-CYD-2432S028R)

---

### [ESP32 CYD Display together with Touch Control and SD Card Reader](https://github.com/AndroidCrypto/ESP32_CYD_Display_with_Touch_and_SD_Card)

Здесь 2 ценных работающих примера с чтением графических файлов 240*320 с SD-карты и вывод их на экран дисплея с управлением от сенсорной панели:


#### [1. Esp32_CYD_TFT_eSPI_SD_with_Touch](ESP32_CYD_Display_with_Touch_and_SD_Card-main/ESP32_CYD_Display_with_Touch_and_SD_Card-main/Esp32_CYD_TFT_eSPI_SD_with_Touch/Esp32_CYD_TFT_eSPI_SD_with_Touch.ino)

#### [2. Esp32_CYD_LovyanGFX_SD_with_Touch](ESP32_CYD_Display_with_Touch_and_SD_Card-main/ESP32_CYD_Display_with_Touch_and_SD_Card-main/Esp32_CYD_LovyanGFX_SD_with_Touch/Esp32_CYD_LovyanGFX_SD_with_Touch.ino)

---

### Рабочие библиотеки выше представленных примеров

#### [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)

#### [TJpg_Decoder](https://github.com/Bodmer/TJpg_Decoder)

#### [XPT2046_Touchscreen](https://github.com/PaulStoffregen/XPT2046_Touchscreen)

#### [LovyanGFX](https://github.com/lovyan03/LovyanGFX)

Библиотека графики для дисплеев (LCD / OLED / EPD) (для ESP32 SPI, I2C, 8bitParallel / ESP8266 SPI, I2C / ATSAMD51 SPI).
M5Stack / M5StickC / TTGO T-Watch / ODROID-GO / ESP-WROVER-KIT / WioTerminal / и другие...

***[Пример: 1_simple_use](1_simple_use/1_simple_use.ino)***

Графическая библиотека, работающая с комбинацией ESP32 с SPI, I2C, 8-битным параллельным подключением / ESP8266 с SPI / ATSAMD51 с SPI и дисплеем.
Это графическая библиотека, которая работает с комбинацией ESP32 с SPI, I2C, 8-битным параллельным подключением / ESP8266 с SPI / ATSAMD51 с SPI и дисплеем. (см. список совместимости ниже).

AdafruitGFX и TFT_eSPI в некоторой степени совместимы, но мы стремимся к более широкому функциональному охвату и высокой производительности.

Эта библиотека имитирует API AdafruitGFX и TFT_eSPI, но при этом обеспечивает более широкий функционал и высокую производительность.

Эта библиотека обладает следующими преимуществами.

- Поддерживаются ArduinoESP32 и ESP-IDF.

- Поддерживаются как 16-битный, так и 24-битный цветовые режимы. (фактическое количество цветов зависит от характеристик дисплея)

- Выполнение другого процесса во время операции связи с использованием прямого доступа к памяти.

- Быстрая ротация/расширение внеэкранного буфера (спрайта).

- Одновременное использование нескольких дисплеев.

- Автоматическая обработка рисования с уменьшением количества цветов для монохромных дисплеев.

- OpenCV, SDL2 можно использовать в качестве места назначения для рисования, и они могут работать на ПК.

- Выход композитного видеосигнала (NTSC, PAL) (только для ESP32).

### [CYD Multi-Page Touch Menu](https://github.com/Carlo47/CYD_MultipageMenu)

В этой примерной программе показано, как реализовать многостраничное сенсорное меню для вызова различных действий. Выбранный пункт меню подсвечивается. При повторном нажатии на выбранный пункт меню выполняется назначенное действие. Другие пункты меню отображаются при смахивании (вверх или вниз).

Для управления меню реализован сенсорный обработчик, который может различать события клик, длительный клик и свайп в четырех направлениях вверх, вниз, влево и вправо. Для этого установлены пользовательские обратные вызовы onShortClick, onLongClick, onSwipeUp, onSwipeDown, onSwipeLeft и onSwipeRight. Программа получает время с сервера NTP, поэтому для ее работы требуется подключение к Wi-Fi. Поэтому SSID и пароль от роутера необходимо ввести в файле initWiFi.cpp.

### [CYD_Projects](https://github.com/bitbank2/CYD_Projects)

Коллекция скетчей Arduino для различных печатных плат ESP32+LCD, которые мы все вместе называем «дешёвыми жёлтыми дисплеями». Не все проекты рассчитаны на все варианты CYD. Два моих любимых варианта — это оригинальный (2,8 дюйма, ILI9341 с резистивным сенсорным экраном) и JC4827W543 (4,3 дюйма, 480x270 S3 с резистивным сенсорным экраном). Пожалуйста, не стесняйтесь добавлять свои проекты и улучшать документацию.

Для каждого проекта обязательно указывайте правильный тип дисплея (постоянное значение, используемое в моей библиотеке bb_spi_lcd).
Вот список наиболее распространенных аббревиатур:

```
DISPLAY_CYD      (оригинальный 2,8 " с одним портом Micro-USB)
DISPLAY_CYD_2USB (более новый 2,8 " с 2 портами USB)
DISPLAY_CYD_22C  (2,2 "ST7789 с емкостным сенсорным экраном)
DISPLAY_CYD_24R  (2,4 "ST7789 с резистивным сенсорным экраном)
DISPLAY_CYD_543  (4,3 "JC4827W543 с резистивным или емкостным сенсорным экраном)
DISPLAY_CYD_128  (1,28 "круглый ESP32-C3 с емкостным сенсорным экраном)
DISPLAY_CYD_35   (3,5 "480x320 Вт с емкостным сенсорным экраном)
DISPLAY_CYD_28C  (2,8 "320x240 Вт /емкостный сенсорный дисплей)
```

### [bb_spi_lcd](https://github.com/bitbank2/bb_spi_lcd?ysclid=ms6ak3y3fh827892779)


### [cyd Here are 161 public repositories matching this topic](https://github.com/topics/cyd)

