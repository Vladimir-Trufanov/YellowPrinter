// EraseFiles
// [ESP32 Arduino: removing a file from the SPIFFS file system]
// (https://techtutorialsx.wordpress.com/2019/02/24/esp32-arduino-removing-a-file-from-the-spiffs-file-system/)

#include "SPIFFS.h"

void listAllFiles(){

  File root = SPIFFS.open("/");

  File file = root.openNextFile();

  while(file){

      Serial.print("FILE: ");
      Serial.println(file.name());

      file = root.openNextFile();
  }

}

void setup() {

  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  File file = SPIFFS.open("/test.txt", FILE_WRITE);

  if (!file) {
    Serial.println("There was an error opening the file for writing");
    return;
  }

  if (file.print("some content")) {
    Serial.println("File was written");
  } else {
    Serial.println("File write failed");
  }

  file.close();

  Serial.println("\n\n---BEFORE REMOVING---");
  listAllFiles();

  SPIFFS.remove("/test.txt");
  SPIFFS.remove("/HuaweiSans24.vlw");
  SPIFFS.remove("/nasalization48.vlw");
  SPIFFS.remove("/HuaweiSans16.vlw");
  SPIFFS.remove("/test.txt");

  Serial.println("\n\n---AFTER REMOVING---");
  listAllFiles();

}

void loop() {}
