// MemoryAndOther.ino - скетч для вывода параметров контроллера
//
// Для получения информации следует откомпилировать скетч
// с настройкой Core Debug Level = Verbose и другими настройками по усмотрению.
// Запустить приложение и снять информацию из последовательного порта

// Подсчитать число символов UTF-8 в последовательности char* 
size_t utf8len(const char* str); 
// Показать состояние памяти с заданным префиксом  
void saymem(char* text); 

void setup()
{
  Serial.begin(115200);
  saymem("setup");
}

void loop() 
{
  saymem("loop");
  delay(3000);
}

#define say(format, ...) \
  { \
    char buffer[256]; \
    snprintf(buffer, sizeof(buffer), format, ##__VA_ARGS__); \
    Serial.print(buffer); \
  }

#define sayln(format, ...) \
  { \
    char buffer[256]; \
    snprintf(buffer, sizeof(buffer), format, ##__VA_ARGS__); \
    Serial.println(buffer); \
  }
  
// ****************************************************************************
// *              Показать состояние памяти с заданным префиксом              *
// ****************************************************************************
void saymem(char* text) 
{
  // Определяем наибольшее число символов текста вместе с пробелами
  // и дополняем текст пробелами слева 
  int nfill=48;   
  int j=nfill-utf8len(text)-2;
  for (int i = 0; i<j; i++) 
  {
    say(" "); 
  }
  // Выводим специальное сообщение по памяти
  /**
   * xPortGetCoreID()        - функция возвращает номер ядра, на котором выполняется текущая задача
   * uxTaskPriorityGet(NULL) - возвращает приоритет текущей задачи (задачи, из которой была вызвана функция)
   * ESP.getFreeHeap()       - возвращает размер свободной кучи (heap) в байтах
   * ESP.getHeapSize()       - возвращает полный размер внутренней оперативной памяти в байтах (ОЗУ)
   * ESP.getFreePsram()      - свободный объём внешней оперативной памяти PSRAM
   * ESP.getPsramSize()      - полный объём внешней оперативной памяти PSRAM
  **/
  say("[%s]",text);
  say(" ядро: %d,",          int(xPortGetCoreID()));        say(" приоритет: %d, ", int(uxTaskPriorityGet(NULL)));
  say("свободная куча %6d ", uint32_t(ESP.getFreeHeap()));  say("от ОЗУ %6d, ",     uint32_t(ESP.getHeapSize()));
  say("FreePSRAM %6d ",      uint32_t(ESP.getFreePsram())); say("от FLASH %6d",     uint32_t(ESP.getPsramSize()));
  sayln(" ");
}
// ****************************************************************************
// *        Подсчитать число символов UTF-8 в последовательности char*        *
// * https://stackoverflow.com/questions/4063146/getting-the-actual-length-of-a-utf-8-encoded-stdstring
// ****************************************************************************
size_t utf8len(const char* str) 
{
  size_t len = 0;
  for (size_t i = 0; *str != 0; ++len) 
  {
    int v01 = ((*str & 0x80) >> 7) & ((*str & 0x40) >> 6);
    int v2 = (*str & 0x20) >> 5;
    int v3 = (*str & 0x10) >> 4;
    str += 1 + ((v01 << v2) | (v01 & v3));
  }
  return len;
}


/**
 ESP32-2432S028R CYD
 -------------------
 CPU Frequence:    "240MHz (WiFi/BT)"
 Flash Frequence:  "80MHz"
 Flash Mode:       "QIO"
 Flash Size:       "4MB"
 Partition Scheme: "Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)"

------------------------------------------
Chip Info:
------------------------------------------
  Model             : ESP32
  Package           : D0WD-Q5
  Revision          : 3.00
  Cores             : 2
  CPU Frequency     : 240 MHz
  XTAL Frequency    : 40 MHz
  Features Bitfield : 0x00000032
  Embedded Flash    : No
  Embedded PSRAM    : No
  2.4GHz WiFi       : Yes
  Classic BT        : Yes
  BT Low Energy     : Yes
  IEEE 802.15.4     : No
------------------------------------------
INTERNAL Memory Info:
------------------------------------------
  Total Size        :   380612 B ( 371.7 KB)
  Free Bytes        :   339108 B ( 331.2 KB)
  Allocated Bytes   :    34196 B (  33.4 KB)
  Minimum Free Bytes:   333468 B ( 325.7 KB)
  Largest Free Block:   110580 B ( 108.0 KB)
  =========== After Setup Start ===========
  Total Size        :   380612 B ( 371.7 KB)
  Free Bytes        :   339020 B ( 331.1 KB)
  Allocated Bytes   :    34252 B (  33.4 KB)
  Minimum Free Bytes:   333120 B ( 325.3 KB)
  Largest Free Block:   110580 B ( 108.0 KB)
------------------------------------------
Flash Info:
------------------------------------------
  Chip Size         :  4194304B (4 MB)
  Block Size        :    65536B (  64.0 KB)
  Sector Size       :     4096B (   4.0 KB)
  Page Size         :      256B (   0.2 KB)
  Bus Speed         : 80 MHz
  Flash Frequency   : 80 MHz (source: 80 MHz, divider: 1)
  Bus Mode          : QIO
------------------------------------------
Partitions Info:
------------------------------------------
                nvs : addr: 0x00009000, size:    20.0 KB, type: DATA, subtype: NVS
            otadata : addr: 0x0000E000, size:     8.0 KB, type: DATA, subtype: OTA
               app0 : addr: 0x00010000, size:  1280.0 KB, type:  APP, subtype: OTA_0
               app1 : addr: 0x00150000, size:  1280.0 KB, type:  APP, subtype: OTA_1
             spiffs : addr: 0x00290000, size:  1408.0 KB, type: DATA, subtype: SPIFFS
           coredump : addr: 0x003F0000, size:    64.0 KB, type: DATA, subtype: COREDUMP
------------------------------------------
Software Info:
------------------------------------------
  Compile Date/Time : Jul  5 2026 14:11:47
  Compile Host OS   : windows
  ESP-IDF Version   : v5.5.4
  Arduino Version   : 3.3.8
------------------------------------------
Board Info:
------------------------------------------
  Arduino Board     : ESP32_2432S028R
  Arduino Variant   : jczn_2432s028r
  Arduino FQBN      : esp32:esp32:jczn_2432s028r:JTAGAdapter=default,PartitionScheme=default,
                      CPUFreq=240,FlashMode=qio,FlashFreq=80,FlashSize=4M,UploadSpeed=921600,
                      LoopCore=1,EventsCore=1,DebugLevel=verbose,EraseFlash=none,ZigbeeMode=default
------------------------------------------
**/

/**
 AI-Thinker ESP32-CAM
 --------------------
 CPU Frequence:    "240MHz (WiFi/BT)"
 Flash Frequence:  "80MHz"
 Flash Mode:       "QIO"
 Flash Size:       "4MB"
 Partition Scheme: "Regular 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)"

------------------------------------------
Chip Info:
------------------------------------------
  Model             : ESP32
  Package           : D0WD-Q6
  Revision          : 1.01
  Cores             : 2
  CPU Frequency     : 240 MHz
  XTAL Frequency    : 40 MHz
  Features Bitfield : 0x00000032
  Embedded Flash    : No
  Embedded PSRAM    : No
  2.4GHz WiFi       : Yes
  Classic BT        : Yes
  BT Low Energy     : Yes
  IEEE 802.15.4     : No
------------------------------------------
INTERNAL Memory Info:
------------------------------------------
  Total Size        :   375820 B ( 367.0 KB)
  Free Bytes        :   334028 B ( 326.2 KB)
  Allocated Bytes   :    34420 B (  33.6 KB)
  Minimum Free Bytes:   328132 B ( 320.4 KB)
  Largest Free Block:   110580 B ( 108.0 KB)
  =========== After Setup Start ============
  Total Size        :   375820 B ( 367.0 KB)
  Free Bytes        :   333940 B ( 326.1 KB)
  Allocated Bytes   :    34476 B (  33.7 KB)
  Minimum Free Bytes:   327880 B ( 320.2 KB)
  Largest Free Block:   110580 B ( 108.0 KB)
------------------------------------------
SPIRAM Memory Info:
------------------------------------------
  Total Size        :  4194304 B (4096.0 KB)
  Free Bytes        :  4192124 B (4093.9 KB)
  Allocated Bytes   :        0 B (   0.0 KB)
  Minimum Free Bytes:  4192124 B (4093.9 KB)
  Largest Free Block:  4128756 B (4032.0 KB)
  Bus Mode          : QSPI
  =========== After Setup Start ============
  Total Size        :  4194304 B (4096.0 KB)
  Free Bytes        :  4192124 B (4093.9 KB)
  Allocated Bytes   :        0 B (   0.0 KB)
  Minimum Free Bytes:  4192124 B (4093.9 KB)
  Largest Free Block:  4128756 B (4032.0 KB)
------------------------------------------
Flash Info:
------------------------------------------
  Chip Size         :  4194304B (4 MB)
  Block Size        :    65536B (  64.0 KB)
  Sector Size       :     4096B (   4.0 KB)
  Page Size         :      256B (   0.2 KB)
  Bus Speed         : 80 MHz
  Flash Frequency   : 80 MHz (source: 80 MHz, divider: 1)
  Bus Mode          : QIO
------------------------------------------
Partitions Info:
------------------------------------------
                nvs : addr: 0x00009000, size:    20.0 KB, type: DATA, subtype: NVS
            otadata : addr: 0x0000E000, size:     8.0 KB, type: DATA, subtype: OTA
               app0 : addr: 0x00010000, size:  1280.0 KB, type:  APP, subtype: OTA_0
               app1 : addr: 0x00150000, size:  1280.0 KB, type:  APP, subtype: OTA_1
             spiffs : addr: 0x00290000, size:  1408.0 KB, type: DATA, subtype: SPIFFS
           coredump : addr: 0x003F0000, size:    64.0 KB, type: DATA, subtype: COREDUMP
------------------------------------------
Software Info:
------------------------------------------
  Compile Date/Time : Jul  5 2026 15:25:45
  Compile Host OS   : windows
  ESP-IDF Version   : v5.5.4
  Arduino Version   : 3.3.8
------------------------------------------
Board Info:
------------------------------------------
  Arduino Board     : ESP32_DEV
  Arduino Variant   : esp32
  Arduino FQBN      : esp32:esp32:esp32cam:CPUFreq=240,FlashMode=qio,PartitionScheme=default,
                      FlashFreq=80,DebugLevel=verbose,EraseFlash=none
------------------------------------------
**/
