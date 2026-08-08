
#include <LovyanGFX.hpp>

// Пример настройки при использовании LovyanGFX с вашими собственными настройками на ESP32

// Создайте класс, который создает ваши собственные настройки, производные от LGFX_Device.
class LGFX : public lgfx::LGFX_Device
{
/*
 Вы можете изменить имя класса с "LGFX" на другое.
 ПРИ ИСПОЛЬЗОВАНИИ ФУНКЦИИ АВТОМАТИЧЕСКОГО ОПРЕДЕЛЕНИЯ ИСПОЛЬЗУЕТСЯ "LGFX", ПОЭТОМУ ИЗМЕНИТЕ ЕГО НА ДРУГОЕ ИМЯ, ОТЛИЧНОЕ ОТ LGFX.
 
 Кроме того, если вы хотите использовать несколько панелей одновременно, присвойте каждой из них другое имя.
 При изменении имени класса имя конструктора также должно быть изменено на то же имя.

 Вы можете сами решить, как его назвать, но предполагайте, что значение параметра будет увеличено.、
 Например, если вы настроите SPI-соединение ILI9341 с помощью ESP32 DevKit-C、LGFX_DevKitC_SPI_ILI9341
 Благодаря совпадению имени файла и имени класса будет трудно заблудиться при его использовании.
*/


// Подготовьте экземпляр, соответствующий типу панели, к которой вы хотите подключиться.
//lgfx::Panel_GC9A01      _panel_instance;
//lgfx::Panel_GDEW0154M09 _panel_instance;
//lgfx::Panel_HX8357B     _panel_instance;
//lgfx::Panel_HX8357D     _panel_instance;
//lgfx::Panel_ILI9163     _panel_instance;
  lgfx::Panel_ILI9341     _panel_instance;
//lgfx::Panel_ILI9342     _panel_instance;
//lgfx::Panel_ILI9481     _panel_instance;
//lgfx::Panel_ILI9486     _panel_instance;
//lgfx::Panel_ILI9488     _panel_instance;
//lgfx::Panel_IT8951      _panel_instance;
//lgfx::Panel_RA8875      _panel_instance;
//lgfx::Panel_SH110x      _panel_instance; // SH1106, SH1107
//lgfx::Panel_SSD1306     _panel_instance;
//lgfx::Panel_SSD1327     _panel_instance;
//lgfx::Panel_SSD1331     _panel_instance;
//lgfx::Panel_SSD1351     _panel_instance; // SSD1351, SSD1357
//lgfx::Panel_SSD1963     _panel_instance;
//lgfx::Panel_ST7735      _panel_instance;
//lgfx::Panel_ST7735S     _panel_instance;
//lgfx::Panel_ST7789      _panel_instance;
//lgfx::Panel_ST7796      _panel_instance;


// Подготовьте экземпляр, соответствующий типу шины, к которой вы хотите подключить панель
  lgfx::Bus_SPI        _bus_instance;   // SPIバスのインスタンス
//lgfx::Bus_I2C        _bus_instance;   // I2Cバスのインスタンス
//lgfx::Bus_Parallel8  _bus_instance;   // 8ビットパラレルバスのインスタンス

// Если возможно управлять подсветкой, подготовьте экземпляр.(Удалите, если в этом нет необходимости)
lgfx::Light_PWM     _light_instance;

// Подготовьте экземпляр, соответствующий типу сенсорного экрана.(Удалите, если в этом нет необходимости)
//lgfx::Touch_CST816S          _touch_instance;
// закомментировал 08.08.2026  lgfx::Touch_FT5x06           _touch_instance; // FT5206, FT5306, FT5406, FT6206, FT6236, FT6336, FT6436
lgfx::Touch_FT5x06           _touch_instance; // FT5206, FT5306, FT5406, FT6206, FT6236, FT6336, FT6436
//lgfx::Touch_GSL1680E_800x480 _touch_instance; // GSL_1680E, 1688E, 2681B, 2682B
//lgfx::Touch_GSL1680F_800x480 _touch_instance;
//lgfx::Touch_GSL1680F_480x272 _touch_instance;
//lgfx::Touch_GSLx680_320x320  _touch_instance;
//lgfx::Touch_GT911            _touch_instance;
//lgfx::Touch_STMPE610         _touch_instance;
//lgfx::Touch_TT21xxx          _touch_instance; // TT21100
//lgfx::Touch_XPT2046          _touch_instance;

public:

  // Создайте конструктор и настройте различные параметры здесь
  // Если вы меняете имя класса, укажите то же имя, что и у конструктора.
  LGFX(void)
  {
    { // Настройте параметры управления шиной
      auto cfg = _bus_instance.config();    // Возвращает структуру для настройки шины.

// SPI - Конфигурация шины
      
    //cfg.spi_host = VSPI_HOST;     // Выберите используемый SPI  ESP32-S2,C3 : SPI2_HOST или SPI3_HOST / ESP32 : VSPI_HOST или HSPI_HOST
      cfg.spi_host = HSPI_HOST;     // Выберите используемый SPI  ESP32-S2,C3 : SPI2_HOST или SPI3_HOST / ESP32 : VSPI_HOST или HSPI_HOST
      
      // С обновлением версии ESP-IDF описание VSPI_HOST и HSPI_HOST станет устаревшим, поэтому, если вы получите сообщение об ошибке, используйте вместо него SPI2_HOST и SPI3_HOST.
      cfg.spi_mode = 0;             // Установите режим связи SPI (от 0 до 3)

    //cfg.freq_write = 40000000;    // Тактовая частота SPI при передаче (до 80 МГц, округленная до 80 МГц, деленная на целое число)
    //cfg.freq_read  = 16000000;    // 受信時のSPIクロック
      cfg.freq_write = 55000000;    // Тактовая частота SPI при передаче (до 80 МГц, округленная до 80 МГц, деленная на целое число)
      cfg.freq_read  = 20000000;    // 受信時のSPIクロック

      cfg.spi_3wire  = true;        // Установите значение true, если прием осуществляется по pin-коду MOSI.
      
    //cfg.use_lock   = true;        // Установите значение true, чтобы использовать блокировку транзакций
      cfg.use_lock   = false;       // Установите значение true, чтобы использовать блокировку транзакций
      
      cfg.dma_channel = SPI_DMA_CH_AUTO; // Установите используемый канал DMA (0=Нет DMA / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=автоматическая настройка).
      // * С обновлением версии ESP-IDF функция SPI_DMA_CH_AUTO (автоматическая настройка) стала рекомендоваться для каналов DMA.Спецификации 1ch и 2ch устарели.
    //cfg.pin_sclk = 18;            // Установите pin-код SCLK для SPI.
      cfg.pin_sclk = 14;            // Установите pin-код SCLK для SPI.
    //cfg.pin_mosi = 23;            // SPIのMOSIピン番号を設定
      cfg.pin_mosi = 13;            // SPIのMOSIピン番号を設定
    //cfg.pin_miso = 19;            // SPIのMISOピン番号を設定 (-1 = disable)
      cfg.pin_miso = 12;            // SPIのMISOピン番号を設定 (-1 = disable)
    //cfg.pin_dc   = 27;            // SPIのD/Cピン番号を設定  (-1 = disable)
      cfg.pin_dc   = 2;            // SPIのD/Cピン番号を設定  (-1 = disable)
     // При использовании шины SPI, общей для SD-карты, обязательно установите MISO, не пропуская его.
//*/
/*
// I2Cバスの設定
      cfg.i2c_port    = 0;          // 使用するI2Cポートを選択 (0 or 1)
      cfg.freq_write  = 400000;     // 送信時のクロック
      cfg.freq_read   = 400000;     // 受信時のクロック
      cfg.pin_sda     = 21;         // SDAを接続しているピン番号
      cfg.pin_scl     = 22;         // SCLを接続しているピン番号
      cfg.i2c_addr    = 0x3C;       // I2Cデバイスのアドレス
//*/
/*
// 8ビットパラレルバスの設定
      cfg.i2s_port = I2S_NUM_0;     // 使用するI2Sポートを選択 (I2S_NUM_0 or I2S_NUM_1) (ESP32のI2S LCDモードを使用します)
      cfg.freq_write = 20000000;    // 送信クロック (最大20MHz, 80MHzを整数で割った値に丸められます)
      cfg.pin_wr =  4;              // WR を接続しているピン番号
      cfg.pin_rd =  2;              // RD を接続しているピン番号
      cfg.pin_rs = 15;              // RS(D/C)を接続しているピン番号
      cfg.pin_d0 = 12;              // D0を接続しているピン番号
      cfg.pin_d1 = 13;              // D1を接続しているピン番号
      cfg.pin_d2 = 26;              // D2を接続しているピン番号
      cfg.pin_d3 = 25;              // D3を接続しているピン番号
      cfg.pin_d4 = 17;              // D4を接続しているピン番号
      cfg.pin_d5 = 16;              // D5を接続しているピン番号
      cfg.pin_d6 = 27;              // D6を接続しているピン番号
      cfg.pin_d7 = 14;              // D7を接続しているピン番号
//*/

      _bus_instance.config(cfg);    // Отражает установленное значение на шине.
      _panel_instance.setBus(&_bus_instance);      // Подключите шину к панели.
    }

    { // Установите управление на панели дисплея.
      auto cfg = _panel_instance.config();    // Возвращает структуру настроек панели отображения.

    //cfg.pin_cs           =    14;  // Pin-код, к которому подключен CS (-1 = отключить)
      cfg.pin_cs           =    15;  // Pin-код, к которому подключен CS (-1 = отключить)
    //cfg.pin_rst          =    33;  // RSTが接続されているピン番号  (-1 = disable)
      cfg.pin_rst          =    -1;  // RSTが接続されているピン番号  (-1 = disable)
      cfg.pin_busy         =    -1;  // BUSYが接続されているピン番号 (-1 = disable)

      // * Следующие настройки имеют общие значения по умолчанию для каждой панели, поэтому, пожалуйста, закомментируйте неизвестные элементы и попробуйте их.

      cfg.panel_width      =   240;  // 実際に表示可能な幅
      cfg.panel_height     =   320;  // 実際に表示可能な高さ
      cfg.offset_x         =     0;  // パネルのX方向オフセット量
      cfg.offset_y         =     0;  // パネルのY方向オフセット量
      cfg.offset_rotation  =     0;  // 回転方向の値のオフセット 0~7 (4~7は上下反転)
      cfg.dummy_read_pixel =     8;  // ピクセル読出し前のダミーリードのビット数
      cfg.dummy_read_bits  =     1;  // ピクセル以外のデータ読出し前のダミーリードのビット数
      cfg.readable         =  true;  // データ読出しが可能な場合 trueに設定
      cfg.invert           = false;  // パネルの明暗が反転してしまう場合 trueに設定
      cfg.rgb_order        = false;  // パネルの赤と青が入れ替わってしまう場合 trueに設定
      cfg.dlen_16bit       = false;  // 16bitパラレルやSPIでデータ長を16bit単位で送信するパネルの場合 trueに設定
      cfg.bus_shared       =  true;  // SDカードとバスを共有している場合 trueに設定(drawJpgFile等でバス制御を行います)

// 以下はST7735やILI9163のようにピクセル数が可変のドライバで表示がずれる場合にのみ設定してください。
//    cfg.memory_width     =   240;  // ドライバICがサポートしている最大の幅
//    cfg.memory_height    =   320;  // ドライバICがサポートしている最大の高さ

      _panel_instance.config(cfg);
    }

//*
    { // Установите регулятор подсветки.(Удалите, если в этом нет необходимости)
      auto cfg = _light_instance.config();    // バックライト設定用の構造体を取得します。

    //cfg.pin_bl = 32;              // バックライトが接続されているピン番号
      cfg.pin_bl = 21;              // バックライトが接続されているピン番号
      cfg.invert = false;           // バックライトの輝度を反転させる場合 true
      cfg.freq   = 44100;           // バックライトのPWM周波数
      cfg.pwm_channel = 7;          // Номер используемого канала ШИМ

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);  // バックライトをパネルにセットします。
    }
//*/

/*
    { // Установите параметры управления с помощью сенсорного экрана.(Удалите, если в этом нет необходимости)
      auto cfg = _touch_instance.config();

      cfg.x_min      = 0;    // タッチスクリーンから得られる最小のX値(生の値)
      cfg.x_max      = 239;  // タッチスクリーンから得られる最大のX値(生の値)
      cfg.y_min      = 0;    // タッチスクリーンから得られる最小のY値(生の値)
      cfg.y_max      = 319;  // タッチスクリーンから得られる最大のY値(生の値)
    //cfg.pin_int    = 38;   // INT - Подключен к пин-коду
      cfg.pin_int    = 36;   // 2 INT - Подключен к пин-коду
      cfg.bus_shared = true; // 画面と共通のバスを使用している場合 trueを設定
      cfg.offset_rotation = 0;// 表示とタッチの向きのが一致しない場合の調整 0~7の値で設定

// SPI接続の場合
    //cfg.spi_host = VSPI_HOST;// 使用するSPIを選択 (HSPI_HOST or VSPI_HOST)
      cfg.spi_host = HSPI_HOST;// 使用するSPIを選択 (HSPI_HOST or VSPI_HOST)
      cfg.freq = 1000000;     // SPIクロックを設定
    //cfg.pin_sclk = 18;     // SCLKが接続されているピン番号
      cfg.pin_sclk = 13; //25;     // SCLKが接続されているピン番号
    //cfg.pin_mosi = 23;     // MOSIが接続されているピン番号
      cfg.pin_mosi = 11; //32;     // MOSIが接続されているピン番号
    //cfg.pin_miso = 19;     // MISOが接続されているピン番号
      cfg.pin_miso = 12; //39;     // MISOが接続されているピン番号
    //cfg.pin_cs   =  5;     //   CSが接続されているピン番号
      cfg.pin_cs   = 8; //33;     //   CSが接続されているピン番号

// I2C接続の場合
      cfg.i2c_port = 1;      // 使用するI2Cを選択 (0 or 1)
      cfg.i2c_addr = 0x38;   // I2Cデバイスアドレス番号
      cfg.pin_sda  = 23;     // SDAが接続されているピン番号
      cfg.pin_scl  = 32;     // SCLが接続されているピン番号
      cfg.freq = 400000;     // I2Cクロックを設定

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);  // タッチスクリーンをパネルにセットします。
    }
*/

    setPanel(&_panel_instance); // 使用するパネルをセットします。
  }
};

// 準備したクラスのインスタンスを作成します。
LGFX display;

void setup(void)
{
  // SPIバスとパネルの初期化を実行すると使用可能になります。
  display.init();

  display.setTextSize((std::max(display.width(), display.height()) + 255) >> 8);

  // タッチが使用可能な場合のキャリブレーションを行います。（省略可）
  //if (display.touch())
  //{
    if (display.width() < display.height()) display.setRotation(display.getRotation() ^ 1);

    // 画面に案内文章を描画します。
    display.setTextDatum(textdatum_t::middle_center);
    display.drawString("touch2the arrow marker.", display.width()>>1, display.height() >> 1);
    display.setTextDatum(textdatum_t::top_left);

    // タッチを使用する場合、キャリブレーションを行います。画面の四隅に表示される矢印の先端を順にタッチしてください。
    std::uint16_t fg = TFT_WHITE;
    std::uint16_t bg = TFT_BLACK;
    if (display.isEPD()) std::swap(fg, bg);
    display.calibrateTouch(nullptr, fg, bg, std::max(display.width(), display.height()) >> 3);
  //}

  delay(3000);
  display.fillScreen(TFT_BLACK);
}

uint32_t count = ~0;
void loop(void)
{
  
  display.startWrite();
  display.setRotation(++count & 7);
  display.setColorDepth((count & 8) ? 16 : 24);

  display.setTextColor(TFT_WHITE);
  display.drawNumber(display.getRotation(), 16, 0);

  display.setTextColor(0xFF0000U);
  display.drawString("R", 30, 16);
  display.setTextColor(0x00FF00U);
  display.drawString("G", 40, 16);
  display.setTextColor(0x0000FFU);
  display.drawString("B", 50, 16);

  display.drawRect(30,30,display.width()-60,display.height()-60,count*7);
  display.drawFastHLine(0, 0, 10);

  display.endWrite();

  int32_t x, y;
  //if (display.getTouch(&x, &y)) {
    x=10; y=10;
    display.fillRect(x-2, y-2, 5, 5, count*7);
  //}
  delay(2000);
  
}
