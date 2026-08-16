// SPIFFS_info.ino

#include <ESP.h>
#include <SPIFFS.h>
//#include <FS.h> // Обязательно подключите библиотеку FS для работы с файловой системой
//#include <SD.h> // Обязательно подключите библиотеку SD для ESP32

/*
После AI Thinker ESP32-CAM
ESP.getHeapSize        = 372308
ESP.getFreeHeap        = 324356
ESP.getMinFreeHeap     = 318708
ESP.getMaxAllocHeap    = 110580
ESP.getPsramSize       = 0
ESP.getFreePsram       = 0
ESP.getMinFreePsram    = 0
ESP.getMaxAllocPsram   = 0
ESP.getFlashChipSize   = 4194304
ESP.getSketchSize      = 329312
ESP.getFreeSketchSpace = 3145728

После CYD
ESP.getHeapSize        = 377092
ESP.getFreeHeap        = 329004
ESP.getMinFreeHeap     = 323364
ESP.getMaxAllocHeap    = 110580
ESP.getPsramSize       = 0
ESP.getFreePsram       = 0
ESP.getMinFreePsram    = 0
ESP.getMaxAllocPsram   = 0
ESP.getFlashChipSize   = 4194304
ESP.getSketchSize      = 320896
ESP.getFreeSketchSpace = 1310720
Free space: 2841798 bytes

*/

void setup() 
{
  Serial.begin(115200);
  delay(3000);


  getmems("До");
  // Initialize the SPIFFS file system
  if (!SPIFFS.begin(true)) 
  {
    Serial.println("Initialization failed!");
    return;
  }
  getmems("После");

  
  /*
  // size of the compiled program
  uint32_t program_size = ESP.getSketchSize();
  
  // size of the file system
  uint32_t file_system_size = uint32_t(SPIFFS.totalBytes);
  uint32_t value = (uint32_t)size_t_variable;

  
  // used size of the file system
  uint32_t file_system_used = SPIFFS.usedBytes;
  
  // free size in the flash memory
  uint32_t free_size = ESP.getFlashChipSize() - program_size - file_system_size + file_system_used;
  
  Serial.print("Program size: ");
  Serial.print(program_size);
  Serial.println(" bytes");
  
  Serial.print("File system size: ");
  Serial.print(file_system_size);
  Serial.println(" bytes");
  
  Serial.print("File system used: ");
  Serial.print(file_system_used);
  Serial.println(" bytes");
  
  Serial.print("Free space: ");
  Serial.print(free_size);
  Serial.println(" bytes");
  */
}

void getmems(char* prefix)
{
  Serial.println(prefix);

  // uint32_t getHeapSize();      //total heap size
  Serial.print("ESP.getHeapSize        = "); Serial.println(ESP.getHeapSize());
  
  // uint32_t getFreeHeap();      //available heap
  Serial.print("ESP.getFreeHeap        = "); Serial.println(ESP.getFreeHeap());
  
  // uint32_t getMinFreeHeap();   //lowest level of free heap since boot
  Serial.print("ESP.getMinFreeHeap     = "); Serial.println(ESP.getMinFreeHeap());
  
  // int32_t getMaxAllocHeap();  //largest block of heap that can be allocated at once
  Serial.print("ESP.getMaxAllocHeap    = "); Serial.println(ESP.getMaxAllocHeap());

  // uint32_t getPsramSize();
  Serial.print("ESP.getPsramSize       = "); Serial.println(ESP.getPsramSize());
  
  // uint32_t getFreePsram();
  Serial.print("ESP.getFreePsram       = "); Serial.println(ESP.getFreePsram());
  
  // uint32_t getMinFreePsram();
  Serial.print("ESP.getMinFreePsram    = "); Serial.println(ESP.getMinFreePsram());
  
  // uint32_t getMaxAllocPsram();
  Serial.print("ESP.getMaxAllocPsram   = "); Serial.println(ESP.getMaxAllocPsram());

  // inline uint32_t getCycleCount() __attribute__((always_inline));
  // Serial.print("ESP.getCycleCount      = "); Serial.println(ESP.getCycleCount());
  
  // uint32_t getFlashChipSize();
  Serial.print("ESP.getFlashChipSize   = "); Serial.println(ESP.getFlashChipSize());

  // uint32_t getSketchSize();
  Serial.print("ESP.getSketchSize      = "); Serial.println(ESP.getSketchSize());

  // uint32_t getFreeSketchSpace();
  Serial.print("ESP.getFreeSketchSpace = "); Serial.println(ESP.getFreeSketchSpace());
  // size of the compiled program
  uint32_t program_size = ESP.getSketchSize();

  // size of the file system
  uint32_t file_system_size = (uint32_t) SPIFFS.totalBytes();

  // used size of the file system
  uint32_t file_system_used = (uint32_t) SPIFFS.usedBytes();
  //uint32_t value = (uint32_t)size_t_variable;

  // free size in the flash memory
  uint32_t free_size = ESP.getFlashChipSize() - program_size - file_system_size + file_system_used;
  Serial.print("Free space: "); Serial.print(free_size); Serial.println(" bytes");
 
}

void loop() 
{
  // Do Nothing Here
}
